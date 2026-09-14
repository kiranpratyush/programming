using Experiment.Indexing.Trees;

namespace Experiment.Indexing
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var bst = new Bst();

            bst.Insert(50);
            bst.Insert(30);
            bst.Insert(70);
            bst.Insert(20);
            bst.Insert(40);
            bst.Insert(60);
            bst.Insert(80);

            bst.PrintInOrder();
        }
    }
}
