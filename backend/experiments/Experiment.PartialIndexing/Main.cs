using Godot;

namespace Experiment.PartialIndexing;

/// <summary>
/// A dependency-free, code-drawn animation explaining database partial indexes.
/// Press Space to pause/resume and R to replay.
/// </summary>
public partial class Main : Node2D
{
    private const float Width = 1280f;
    private const float Height = 720f;
    private const float Duration = 25f;

    private static readonly Color Background = new("#09101f");
    private static readonly Color Panel = new("#111c31");
    private static readonly Color PanelSecondary = new("#17243b");
    private static readonly Color Border = new("#2a3b58");
    private static readonly Color Text = new("#f1f5ff");
    private static readonly Color Muted = new("#91a0b8");
    private static readonly Color Blue = new("#63a6ff");
    private static readonly Color Green = new("#4ee1a0");
    private static readonly Color Amber = new("#ffca68");
    private static readonly Color Red = new("#ff718a");

    private static readonly UserRow[] Rows =
    [
        new("101", "ava@example.com", true),
        new("102", "ben@example.com", false),
        new("103", "chen@example.com", true),
        new("104", "dia@example.com", false),
        new("105", "eli@example.com", true),
        new("106", "fay@example.com", false),
        new("107", "gio@example.com", true),
        new("108", "hana@example.com", false)
    ];

    private float _elapsed;
    private bool _paused;
    private Font _font = null!;

    public override void _Ready()
    {
        _font = ThemeDB.FallbackFont;

        // Optional render helper: Godot -- --seek=13.0
        foreach (var argument in OS.GetCmdlineUserArgs())
        {
            if (argument.StartsWith("--seek=", StringComparison.Ordinal))
            {
                var value = argument["--seek=".Length..];
                if (float.TryParse(value, out var seekTime))
                {
                    _elapsed = Mathf.Clamp(seekTime, 0f, Duration - 0.01f);
                }
            }
        }

        QueueRedraw();
    }

    public override void _Process(double delta)
    {
        if (!_paused)
        {
            _elapsed = Mathf.PosMod(_elapsed + (float)delta, Duration);
        }

        QueueRedraw();
    }

    public override void _UnhandledInput(InputEvent @event)
    {
        if (@event.IsActionPressed("ui_accept"))
        {
            _paused = !_paused;
        }
        else if (@event is InputEventKey { Pressed: true, Keycode: Key.R })
        {
            _elapsed = 0f;
            _paused = false;
        }
    }

    public override void _Draw()
    {
        DrawRect(new Rect2(0, 0, Width, Height), Background);
        DrawBackgroundGlow();
        DrawHeader();
        DrawQuery();
        DrawTable();
        DrawIndexPanel();
        DrawFooter();
    }

    private void DrawBackgroundGlow()
    {
        for (var i = 8; i > 0; i--)
        {
            var alpha = 0.007f * (9 - i);
            DrawCircle(new Vector2(1080, 120), i * 58f, WithAlpha(Blue, alpha));
        }
    }

    private void DrawHeader()
    {
        var intro = EaseOut(Window(0f, 0.8f));
        var y = Mathf.Lerp(54f, 78f, intro);
        DrawLabel("PARTIAL INDEXING", new Vector2(64, y), 34, Text, intro);
        DrawLabel("Index only the rows your query actually needs.", new Vector2(66, y + 34), 18, Muted, intro);
        DrawRect(new Rect2(64, y + 48, 265f * intro, 3), Blue);
    }

    private void DrawQuery()
    {
        var progress = EaseOut(Window(1f, 1f));
        var rectangle = new Rect2(64, Mathf.Lerp(135f, 125f, progress), 700, 58);
        DrawCard(rectangle, Panel, Border, progress);
        DrawLabel("QUERY", new Vector2(82, rectangle.Position.Y + 22), 13, Blue, progress);
        DrawLabel("SELECT * FROM users WHERE status = 'active';", new Vector2(82, rectangle.Position.Y + 46), 17, Text, progress);
    }

    private void DrawTable()
    {
        var appear = EaseOut(Window(1.4f, 1f));
        var filtering = EaseInOut(Window(3.4f, 2f));
        var tableRectangle = new Rect2(64, 205, 700, 372);
        DrawCard(tableRectangle, Panel, Border, appear);

        DrawRect(new Rect2(65, 206, 698, 44), WithAlpha(PanelSecondary, appear));
        DrawLabel("id", new Vector2(88, 235), 15, Muted, appear);
        DrawLabel("email", new Vector2(190, 235), 15, Muted, appear);
        DrawLabel("status", new Vector2(625, 235), 15, Muted, appear);

        for (var i = 0; i < Rows.Length; i++)
        {
            var row = Rows[i];
            var rowY = 251f + (i * 40f);
            var rowAlpha = appear;

            if (filtering > 0f && !row.Active)
            {
                rowAlpha *= Mathf.Lerp(1f, 0.22f, filtering);
            }

            if (row.Active)
            {
                DrawRect(new Rect2(66, rowY, 696, 38), WithAlpha(Green, 0.12f * filtering));
                DrawRect(new Rect2(66, rowY, 4, 38), WithAlpha(Green, filtering));
            }

            DrawLabel(row.Id, new Vector2(88, rowY + 25), 15, Text, rowAlpha);
            DrawLabel(row.Email, new Vector2(190, rowY + 25), 15, Text, rowAlpha);
            DrawBadge(row.Active ? "active" : "inactive", new Vector2(619, rowY + 8), row.Active ? Green : Red, rowAlpha);

            if (i < Rows.Length - 1)
            {
                DrawLine(new Vector2(78, rowY + 39), new Vector2(749, rowY + 39), WithAlpha(Border, 0.55f * rowAlpha));
            }
        }

        if (filtering > 0.15f && _elapsed < 9f)
        {
            DrawLabel("Only matching rows qualify", new Vector2(505, 603), 15, Green, EaseOut(Window(4.6f, 0.7f)));
        }
    }

    private void DrawIndexPanel()
    {
        var panelAppear = EaseOut(Window(5.8f, 0.8f));
        var panelRectangle = new Rect2(800, 125, 416, 452);
        DrawCard(panelRectangle, Panel, Border, panelAppear);
        DrawLabel("PARTIAL INDEX", new Vector2(824, 159), 14, Green, panelAppear);
        DrawLabel("idx_active_users", new Vector2(824, 188), 23, Text, panelAppear);

        var sqlAlpha = EaseOut(Window(6.3f, 0.8f));
        DrawRect(new Rect2(820, 205, 376, 67), WithAlpha(new Color("#0b1426"), sqlAlpha));
        DrawLabel("CREATE INDEX ... ON users(email)", new Vector2(836, 230), 14, Blue, sqlAlpha);
        DrawLabel("WHERE status = 'active';", new Vector2(836, 255), 14, Green, sqlAlpha);

        var build = EaseInOut(Window(7f, 3f));
        var activeIndex = 0;

        for (var i = 0; i < Rows.Length; i++)
        {
            var row = Rows[i];
            if (!row.Active)
            {
                continue;
            }

            var localStart = activeIndex * 0.18f;
            var itemProgress = EaseOut(Mathf.Clamp((build - localStart) / 0.55f, 0f, 1f));
            var source = new Vector2(720, 270f + (i * 40f));
            var target = new Vector2(824, 299f + (activeIndex * 54f));
            var position = source.Lerp(target, itemProgress);
            var chipRectangle = new Rect2(position.X, position.Y, Mathf.Lerp(48f, 368f, itemProgress), 42);

            DrawRect(chipRectangle, WithAlpha(Green, 0.10f + (0.06f * itemProgress)));
            DrawRect(chipRectangle, WithAlpha(Green, 0.45f * itemProgress), false, 1.2f);

            if (itemProgress > 0.45f)
            {
                var alpha = Mathf.Remap(itemProgress, 0.45f, 1f, 0f, 1f) * panelAppear;
                DrawLabel(row.Email, position + new Vector2(14, 27), 14, Text, alpha);
                DrawLabel($"→ {row.Id}", position + new Vector2(282, 27), 14, Green, alpha);
            }

            activeIndex++;
        }

        DrawComparison();
    }

    private void DrawComparison()
    {
        var compare = EaseOut(Window(10.8f, 0.8f));
        if (compare <= 0f)
        {
            return;
        }

        DrawRect(new Rect2(812, 281, 392, 285), WithAlpha(Panel, compare));
        DrawLabel("INDEX ENTRIES", new Vector2(824, 310), 13, Muted, compare);
        DrawLabel("Full index", new Vector2(824, 350), 16, Text, compare);
        DrawLabel("8 rows", new Vector2(1120, 350), 16, Amber, compare);
        DrawRect(new Rect2(824, 366, 344f * compare, 18), WithAlpha(Amber, 0.72f * compare));

        var partialBar = EaseOut(Window(11.5f, 1f));
        DrawLabel("Partial index", new Vector2(824, 422), 16, Text, compare);
        DrawLabel("4 rows", new Vector2(1120, 422), 16, Green, partialBar);
        DrawRect(new Rect2(824, 438, 172f * partialBar, 18), WithAlpha(Green, 0.82f * partialBar));

        var saved = EaseOut(Window(12.5f, 0.8f));
        DrawLabel("50% fewer entries in this example", new Vector2(824, 497), 17, Green, saved);
        DrawLabel("Smaller index • fewer irrelevant updates", new Vector2(824, 530), 14, Muted, saved);
        DrawLookup(compare);
    }

    private void DrawLookup(float baseAlpha)
    {
        var lookup = EaseInOut(Window(14.2f, 1.4f));
        if (lookup <= 0f)
        {
            return;
        }

        var start = new Vector2(834, 552);
        var finish = new Vector2(1178, 552);
        DrawLine(start, finish, WithAlpha(Blue, 0.25f * baseAlpha), 3);
        DrawCircle(new Vector2(Mathf.Lerp(start.X, finish.X, lookup), 552), 7, Blue);

        var result = EaseOut(Window(15.4f, 0.7f));
        DrawLabel("Query uses the smaller index", new Vector2(824, 552), 14, Blue, 1f - lookup);
        DrawLabel("MATCH FOUND", new Vector2(1037, 557), 14, Green, result);
    }

    private void DrawFooter()
    {
        var insight = EaseOut(Window(17f, 0.8f));
        DrawCard(new Rect2(64, 606, 1152, 72), new Color("#0d182b"), Border, Mathf.Max(0.35f, insight));

        if (_elapsed < 17f)
        {
            DrawLabel("A partial index stores only rows that satisfy its WHERE predicate.", new Vector2(88, 648), 17, Muted, 0.85f);
        }
        else
        {
            DrawLabel("Best for selective, frequently repeated filters", new Vector2(88, 637), 16, Green, insight);
            DrawLabel("The query predicate must match the index predicate.", new Vector2(88, 661), 15, Text, insight);
        }

        var controls = _paused ? "SPACE  resume" : "SPACE  pause";
        DrawLabel($"{controls}     R  replay", new Vector2(1014, 705), 12, Muted, 0.72f);
    }

    private void DrawCard(Rect2 rectangle, Color fill, Color border, float alpha)
    {
        if (alpha <= 0f)
        {
            return;
        }

        DrawRect(rectangle, WithAlpha(fill, fill.A * alpha));
        DrawRect(rectangle, WithAlpha(border, border.A * alpha), false, 1.5f);
    }

    private void DrawBadge(string label, Vector2 position, Color color, float alpha)
    {
        var badgeRectangle = new Rect2(position, new Vector2(label == "inactive" ? 82 : 68, 24));
        DrawRect(badgeRectangle, WithAlpha(color, 0.12f * alpha));
        DrawRect(badgeRectangle, WithAlpha(color, 0.50f * alpha), false);
        DrawLabel(label, position + new Vector2(9, 17), 12, color, alpha);
    }

    private void DrawLabel(string value, Vector2 position, int size, Color color, float alpha = 1f)
    {
        if (alpha <= 0f)
        {
            return;
        }

        DrawString(_font, position, value, HorizontalAlignment.Left, -1, size, WithAlpha(color, color.A * alpha));
    }

    private float Window(float start, float length) => Mathf.Clamp((_elapsed - start) / length, 0f, 1f);

    private static float EaseOut(float value) => 1f - Mathf.Pow(1f - value, 3f);

    private static float EaseInOut(float value) => value * value * (3f - (2f * value));

    private static Color WithAlpha(Color color, float alpha) => new(color.R, color.G, color.B, alpha);

    private sealed record UserRow(string Id, string Email, bool Active);
}
