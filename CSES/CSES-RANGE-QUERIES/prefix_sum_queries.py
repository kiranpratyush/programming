import sys


# Helper to create a tree item (dictionary in Python)
def make_tree_item(sum_value, prefix_value):
    """Creates a dictionary representing a node in the segment tree."""
    return {"sum_value": sum_value, "prefix": prefix_value}


def build(tree, array, index, start_node_range, end_node_range):
    """
    Builds the segment tree from the input array.

    Args:
        tree: The list representing the segment tree.
        array: The input array of numbers.
        index: The current node's index in the tree (1-based).
        start_node_range: The starting index of the range covered by this node in the original array (0-based).
        end_node_range: The ending index of the range covered by this node in the original array (0-based).
    """
    if start_node_range > end_node_range:
        return

    if start_node_range == end_node_range:
        # Leaf node
        value = array[start_node_range]
        tree[index] = make_tree_item(value, max(0, value))
        return

    mid = (start_node_range + end_node_range) // 2
    # Recursively build left and right children
    build(tree, array, 2 * index, start_node_range, mid)
    build(tree, array, 2 * index + 1, mid + 1, end_node_range)

    # Combine results from children
    left_child_item = tree[2 * index]
    right_child_item = tree[2 * index + 1]

    current_sum_value = left_child_item["sum_value"] + right_child_item["sum_value"]

    # Calculate prefix based on the C++ logic, simplified max:
    # max(0, sum_of_current_segment, left_child_prefix, left_child_sum + right_child_prefix)
    max_prefix_value = max(
        0,
        current_sum_value,
        left_child_item["prefix"],
        left_child_item["sum_value"] + right_child_item["prefix"],
    )

    tree[index] = make_tree_item(current_sum_value, max_prefix_value)


def update(tree, index, update_array_idx, value, start_node_range, end_node_range):
    """
    Updates a value in the array and propagates changes in the segment tree.

    Args:
        tree: The list representing the segment tree.
        index: The current node's index in the tree (1-based).
        update_array_idx: The 0-based index in the original array to update.
        value: The new value to set at update_array_idx.
        start_node_range: The starting index of the range covered by this node.
        end_node_range: The ending index of the range covered by this node.
    """
    if start_node_range > end_node_range:
        return

    if start_node_range == end_node_range:
        # Leaf node
        if start_node_range == update_array_idx:  # Check if this is the node to update
            tree[index] = make_tree_item(value, max(0, value))
        return

    mid = (start_node_range + end_node_range) // 2
    if update_array_idx > mid:
        # Update is in the right child
        update(tree, 2 * index + 1, update_array_idx, value, mid + 1, end_node_range)
    else:
        # Update is in the left child
        update(tree, 2 * index, update_array_idx, value, start_node_range, mid)

    # Recalculate current node's values after child update
    left_child_item = tree[2 * index]
    right_child_item = tree[2 * index + 1]

    current_sum_value = left_child_item["sum_value"] + right_child_item["sum_value"]
    # Simplified max call
    max_prefix_value = max(
        0,
        current_sum_value,
        left_child_item["prefix"],
        left_child_item["sum_value"] + right_child_item["prefix"],
    )

    tree[index] = make_tree_item(current_sum_value, max_prefix_value)


def query(
    tree, index, query_start_range, query_end_range, start_node_range, end_node_range
):
    """
    Queries the segment tree for the maximum prefix sum in the given range [query_start_range, query_end_range].

    Args:
        tree: The list representing the segment tree.
        index: The current node's index in the tree (1-based).
        query_start_range: The starting index of the query range (0-based).
        query_end_range: The ending index of the query range (0-based).
        start_node_range: The starting index of the range covered by this node.
        end_node_range: The ending index of the range covered by this node.

    Returns:
        A dictionary {'sum_value': ..., 'prefix': ...} for the queried range.
    """
    # Case 1: Current node's range is completely outside the query range
    if start_node_range > query_end_range or end_node_range < query_start_range:
        return make_tree_item(0, 0)  # Identity element

    # Case 2: Current node's range is completely inside the query range
    if query_start_range <= start_node_range and end_node_range <= query_end_range:
        return tree[index]

    # Case 3: Partial overlap
    mid = (start_node_range + end_node_range) // 2
    left_result = query(
        tree, 2 * index, query_start_range, query_end_range, start_node_range, mid
    )
    right_result = query(
        tree, 2 * index + 1, query_start_range, query_end_range, mid + 1, end_node_range
    )

    # Combine results from children (same logic as in build/update), simplified max
    combined_sum_value = left_result["sum_value"] + right_result["sum_value"]
    combined_prefix_value = max(
        0,
        combined_sum_value,
        left_result["prefix"],
        left_result["sum_value"] + right_result["prefix"],
    )

    return make_tree_item(combined_sum_value, combined_prefix_value)


def main():
    """
    Main function to read input, build tree, and process queries.
    """
    # Use sys.stdin.readline for faster input, similar to scanf
    # For competitive programming, it's often better.
    # For simplicity, you can also use input().split()

    line1 = sys.stdin.readline().split()
    num_values = int(line1[0])
    num_queries = int(line1[1])

    array_str = sys.stdin.readline().split()
    array = [int(x) for x in array_str]

    # Initialize tree with default values. Size is 4*N for segment trees.
    # Using 1-based indexing for tree nodes, so size is 4*N, effectively index 0 is unused.
    # Or, make it 4*N + 1 if you want to be very precise with 1-based indexing.
    # The C++ code uses tree(4 * num_values), implying 0 to 4*N-1.
    # If root is at index 1, then max index used is roughly 2*N. 4*N is safe.
    tree = [make_tree_item(0, 0) for _ in range(4 * num_values)]

    if num_values > 0:  # Build tree only if there are values
        build(tree, array, 1, 0, num_values - 1)  # Root at index 1, array is 0-indexed

    results_output = []
    for _ in range(num_queries):
        query_line = sys.stdin.readline().split()
        query_type = int(query_line[0])
        param1 = int(query_line[1])
        param2 = int(query_line[2])

        if query_type == 1:
            # Update query: param1 is 1-based index, param2 is the new value
            # Convert array index to 0-based
            if num_values > 0:  # Ensure update is valid if array exists
                update(tree, 1, param1 - 1, param2, 0, num_values - 1)
        else:
            # Range query: param1 is 1-based start, param2 is 1-based end
            # Convert query range to 0-based
            if num_values > 0:  # Ensure query is valid if array exists
                result_item = query(tree, 1, param1 - 1, param2 - 1, 0, num_values - 1)
                results_output.append(str(result_item["prefix"]))
            else:  # Handle query on empty tree
                results_output.append("0")  # Default prefix for empty range

    sys.stdout.write("\n".join(results_output) + "\n")


if __name__ == "__main__":
    main()
