namespace Experiment.Postgres.Models
{
    public class OrderByCustomer
    {
        public required long Id { get; set; }
        public required long OrderId { get; set; }
        public required DateTime PlacedAt { get; set; }

        public required long TotalOrderAmount { get; set; }
    }
    public class OrderSummary
    {
        public required long CustomerId { get; set; }

        public required string CustomerName { get; set; }

        public required long TotalOrderCount { get; set; }

        public required decimal TotalOrderAmount { get; set; }

        public DateTime? LastOrderedAt { get; set; }

        
    }
}
