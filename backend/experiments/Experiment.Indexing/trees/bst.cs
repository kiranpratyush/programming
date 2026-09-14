namespace Experiment.Indexing.Trees
{
    public class Node
    {
        public int Value { get; set; }

        public Node? LeftChild { get; set; }

        public Node? RightChild { get; set; }
    }

    public class Bst
    {
        private Node? _root;

        public void Insert(int value)
        {
            _root = InsertHelper(_root, value);
        }

        public bool Search(int value, out Node? resultNode)
        {
            resultNode = SearchHelper(_root, value);
            return resultNode != null;
        }

        public bool Remove(int value)
        {
            if (!Search(value, out _))
                return false;

            _root = RemoveHelper(_root, value);
            return true;
        }

        private Node InsertHelper(Node? root, int value)
        {
            if (root == null)
            {
                return new Node
                {
                    Value = value
                };
            }

            if (value < root.Value)
            {
                root.LeftChild = InsertHelper(root.LeftChild, value);
            }
            else
            {
                root.RightChild = InsertHelper(root.RightChild, value);
            }

            return root;
        }

        private Node? SearchHelper(Node? root, int value)
        {
            if (root == null)
                return null;

            if (root.Value == value)
                return root;

            if (value < root.Value)
            {
                return SearchHelper(root.LeftChild, value);
            }

            return SearchHelper(root.RightChild, value);
        }

        private Node? RemoveHelper(Node? root, int value)
        {
            if (root == null)
                return null;

            if (value < root.Value)
            {
                root.LeftChild = RemoveHelper(root.LeftChild, value);
                return root;
            }

            if (value > root.Value)
            {
                root.RightChild = RemoveHelper(root.RightChild, value);
                return root;
            }
            if (root.LeftChild == null)
            {
                return root.RightChild;
            }

            if (root.RightChild == null)
            {
                return root.LeftChild;
            }

            var successor = FindSmallest(root.RightChild)!;

            root.Value = successor.Value;

            root.RightChild =
                RemoveHelper(root.RightChild, successor.Value);

            return root;
        }

        private Node? FindSmallest(Node? root)
        {
            if (root == null)
                return null;

            while (root.LeftChild != null)
            {
                root = root.LeftChild;
            }

            return root;
        }

        public void PrintInOrder()
        {
            PrintInOrderHelper(_root);
            Console.WriteLine();
        }

        private void PrintInOrderHelper(Node? root)
        {
            if (root == null)
                return;

            PrintInOrderHelper(root.LeftChild);

            Console.Write($"{root.Value} ");

            PrintInOrderHelper(root.RightChild);
        }
    }
}