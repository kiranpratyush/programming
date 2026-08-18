using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Channels;

namespace Experiment.Types
{
    internal class Channels
    {
        private readonly Channel<int> _channel;

        public Channels()
        {
            _channel = Channel.CreateUnbounded<int>();
        }
        async Task ProduceAsync()
        {
           for (int i = 0; i < 100; i++)
            {
                await _channel.Writer.WriteAsync(i);    
            }
            Console.WriteLine("Waiting for 10 second before calling complete");
            await Task.Delay(10000);
            _channel.Writer.Complete();
        }
        async Task ConsumeAsync()
        {
            await foreach( int item in  _channel.Reader.ReadAllAsync())
            {
                Console.WriteLine($"Received:{item}");
            }
        }
        public async Task Run()
        {
            Task producer = ProduceAsync();
            Task consumer = ConsumeAsync();
            await Task.WhenAll(producer, consumer);
        }
    }
}
