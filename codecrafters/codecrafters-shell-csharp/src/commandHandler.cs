using System.Text;
using src.parser;
using src.utils;
namespace src
{
    public class CommandHandler
    {
        private readonly List<string> _knownCommands = new List<string>
        {
           "echo", "exit", "type", "pwd", "cd"
        };
        private string _buffer;
        private int _cursorIndex;
        private readonly string _prompt;

        private bool _isTabPressed = false;

        public CommandHandler(string prompt = "$ ")
        {
            _prompt = prompt;
            _buffer = "";
            _knownCommands.AddRange(FindCommand.getAllCommands(":"));
            _knownCommands = new HashSet<string>(_knownCommands).ToList();
        }

        public void Run()
        {
            while (true)
            {
                _buffer = "";
                _cursorIndex = 0;
                string input = ReadCommandLine();
                if (string.IsNullOrWhiteSpace(input)) continue;

                var command = CommandParser.Parse(input);
                command.Run();
                Console.Out.Flush();
            }
        }

        private string ReadCommandLine()
        {
            Console.Write(_prompt);
            while (true)
            {
                ConsoleKeyInfo keyInfo = Console.ReadKey(intercept: true);
                if (keyInfo.Key == ConsoleKey.Enter)
                {
                    Console.WriteLine();
                    return _buffer;
                }
                else if (keyInfo.Key == ConsoleKey.LeftArrow)
                {
                    if (_cursorIndex > 0) _cursorIndex--;
                }
                else if (keyInfo.Key == ConsoleKey.RightArrow)
                {
                    if (_cursorIndex < _buffer.Length) _cursorIndex++;
                }
                else if (keyInfo.Key == ConsoleKey.Backspace)
                {
                    if (_cursorIndex > 0)
                    {
                        _cursorIndex--;
                        _buffer = _buffer.Remove(_cursorIndex, 1);
                        Console.Write("\b \b");
                    }
                }
                else if (keyInfo.Key == ConsoleKey.Tab)
                {
                    HandleAutocomplete();
                }
                else if (!char.IsControl(keyInfo.KeyChar))
                {
                    _cursorIndex++;
                    _buffer += keyInfo.KeyChar;
                    Console.Write(keyInfo.KeyChar);

                }
            }
        }
        private void HandleAutocomplete()
        {
            var command = _knownCommands.Where(command => command.StartsWith(_buffer.ToString()));
            if (command.Count() <= 1)
            {
                if (!string.IsNullOrEmpty(command.FirstOrDefault()))
                {
                    _buffer = command.FirstOrDefault() + " ";
                    Console.Write($"\r$ {_buffer}");
                }
                else
                {
                    Console.Write("\a");
                }
                _isTabPressed = false;
            }
            else
            {
                if (!_isTabPressed)
                {
                    // Find LCP here if exist print that out 
                    var common = FindLcp(command);
                    if (!string.IsNullOrEmpty(common) && common.Length > _buffer.ToString().Length)
                    {
                        _buffer = common;
                        Console.Write($"\r$ {_buffer}");
                        _isTabPressed = false;
                    }
                    else
                    {
                        // Else Pint the bell char
                        Console.Write("\a");
                        _isTabPressed = true;
                    }

                }
                else
                {
                    var commands = command.ToList();
                    commands.Sort();
                    Console.WriteLine();
                    Console.WriteLine(string.Join("  ", commands));
                    Console.Write($"$ {_buffer}");
                    _isTabPressed = false;
                }
            }
        }
        private string FindLcp(IEnumerable<string> commands)
        {
            int min_command_length = int.MaxValue;
            string min_command = "";
            StringBuilder x = new StringBuilder();
            foreach (var command in commands)
            {
                if (command.Length < min_command_length)
                {
                    min_command_length = command.Length;
                    min_command = command;
                }
            }
            int i = 0;
            while (i < min_command_length)
            {
                bool shouldBreak = false;
                foreach (var command in commands)
                {
                    if (command[i] != min_command[i])
                    {
                        shouldBreak = true;
                        break;

                    }
                }
                if (shouldBreak)
                {
                    break;
                }
                x.Append(min_command[i]);
                i++;
            }
            return x.ToString();
        }
    }
}