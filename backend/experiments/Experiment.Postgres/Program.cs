using Experiment.Postgres.Models;
using Npgsql;
using System.Data;
using System.Data.Common;
using System.Text;

var builder = WebApplication.CreateBuilder(args);
string connectionString = builder.Configuration.GetConnectionString("postgres")?? throw new InvalidOperationException("Postgres connection string is missing");
builder.Services.AddNpgsqlDataSource(connectionString);
var app = builder.Build();

app.MapGet("/", () => "Hello World!");


app.MapGet("/customers/{customerId}/orders", async (int customerId, DateTime? from, int? limit, NpgsqlDataSource dataSource,
    CancellationToken cancellationToken) =>
{
    var result = new List<OrderByCustomer> { } ;
    await using var command = dataSource.CreateCommand();
    var sql = new StringBuilder("""
        
        SELECT o.customer_id, o.id,o.placed_at,sum(oi.unit_price * oi.quantity) AS total FROM orders o
        INNER JOIN order_items oi on o.id = oi.order_id and o.customer_id = @customerId 
        
        """);
    command.Parameters.AddWithValue("customerId", customerId);
    if (from is not null)
    {
        sql.AppendLine("""AND o.placed_at > @from::date - interval '1  day'""");
        command.Parameters.AddWithValue("from", from);
    }
    sql.AppendLine(""" GROUP BY o.customer_id, o.id,o.placed_at ORDER BY o.id DESC, o.placed_at DESC """);
    if (limit is not null)
    {
        sql.AppendLine("limit @limit");
        command.Parameters.AddWithValue("limit", limit);
    }
    sql.Append(';');
    command.CommandText = sql.ToString();

    await using var reader = await command.ExecuteReaderAsync();
    var customerIdOrdinal = reader.GetOrdinal("customer_id");
    var orderIdOrdinal = reader.GetOrdinal("id");
    var placedAtOrdinal = reader.GetOrdinal("placed_at");
    var totalOrderOrdinal = reader.GetOrdinal("total");
    while(await reader.ReadAsync())
    {
        var customerIdrow = reader.GetInt64(customerIdOrdinal);
        var orderIdrow = reader.GetInt64(orderIdOrdinal);
        var placedAtrow = reader.GetDateTime(placedAtOrdinal);
        var totalOrderrow = reader.GetInt64(totalOrderOrdinal);
        result.Add(new OrderByCustomer { Id = customerId, OrderId = orderIdrow, PlacedAt = placedAtrow, TotalOrderAmount = totalOrderrow });
    }
    return result;
});

app.MapGet("/customers/order-summary", async (string? status,CancellationToken cancellationToken,NpgsqlDataSource npgsqlDataSource ) =>
{
    var result = new List<OrderSummary> { };
    await using var command = npgsqlDataSource.CreateCommand();
    var sql = new StringBuilder("""
        select c.id customerId,c.name customerName,count( distinct result.id) orderCount,coalesce(sum(result.quantity*result.unit_price),0) totalSpent,max(result.placed_at) lastOrderPlacedAt
        from customers c
        left join  (select o.id,oi.quantity,oi.unit_price,o.customer_id,o.placed_at from  orders o
        inner join order_items oi
        on o.id = oi.order_id
        """);
    
    if (!string.IsNullOrWhiteSpace(status))
    {
        sql.AppendLine(" where o.status = @status)");
        command.Parameters.AddWithValue("status", status);
    }
    else
    {
        sql.Append(')');
    }
    sql.AppendLine(
        """
        as result(id,quantity,unit_price,customer_id,placed_at)
        on c.id = result.customer_id
        group by c.id,c.name
        order by ordercount desc,
        customerId;
        """);
    command.CommandText = sql.ToString();

    await using var reader = await command.ExecuteReaderAsync();
    var customerIdOrdinal = reader.GetOrdinal("customerId");
    var customerNameOrdinal = reader.GetOrdinal("customerName");
    var orderCountOrdinal = reader.GetOrdinal("orderCount");
    var totalSpentOrdial = reader.GetOrdinal("totalSpent");
    var lastOrderPlacedAtOrdinal = reader.GetOrdinal("lastOrderPlacedAt");
    while (await reader.ReadAsync())
    {
        var customerIdrow = reader.GetInt64(customerIdOrdinal);
        var customerNamerow = reader.GetString(customerNameOrdinal);
        var orderCountrow = reader.GetInt64(orderCountOrdinal);
        var totalSpentrow = reader.GetDecimal(totalSpentOrdial);
        DateTime? lastOrderPlacedAt = null;
        if(!reader.IsDBNull(lastOrderPlacedAtOrdinal))
        {
            lastOrderPlacedAt = reader.GetDateTime(lastOrderPlacedAtOrdinal);
        }
        
        result.Add(new OrderSummary {
            CustomerId = customerIdrow,
            CustomerName=customerNamerow,
            TotalOrderAmount = totalSpentrow,
            LastOrderedAt = lastOrderPlacedAt,
            TotalOrderCount = orderCountrow,
        });
    }
    return result;


});

app.Run();
