namespace MqttExperiments
{
    class FakeUnManagedResource
    {
        public static int ActiveHandles = 0;

        public FakeUnManagedResource()
        {
            ActiveHandles++;
            Console.WriteLine($"[Alloc] Active handles: {ActiveHandles}");
        }
        public void Release()
        {
            ActiveHandles--;
            Console.WriteLine($"[Delloc] Active Handles: {ActiveHandles}");
        }
    }
    public class BadResourceUser
    {
        private FakeUnManagedResource _resource = new FakeUnManagedResource();

        ~BadResourceUser()
        {
            _resource.Release();
        }
    }
    public class DisposableUnderstanding
    {
        public DisposableUnderstanding()
        {
            for (int i = 0; i < 10000; i++)
            {
                new BadResourceUser();
            }
            while (true)
            {
                Console.WriteLine($"{FakeUnManagedResource.ActiveHandles}");
            }
        }
    }
}