
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_NODES 5
#define NUM_DIGITS 2

typedef struct NODE NODE;

struct NODE {
	int data;
	NODE *left, *right;
};

NODE *addNode(NODE *head, int n) {
	NODE *newNode = malloc(sizeof(NODE));
	newNode->data = n;
	newNode->left = NULL;
	newNode->left = NULL;

	if (head == NULL)
		return newNode;

	NODE *parent = head;
	NODE **next = &head;

	while (*next != NULL) {
		parent = *next;
		next = (rand() % 2) ? &parent->left : &parent->right;
	}
	
	*next = newNode;

	return head;
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

int max3(int a, int b, int c) {
	return max(a, max(b, c));
}

int min(int a, int b) {
	return (a < b) ? a : b;
}

int min3(int a, int b, int c) {
	return min(a, min(b, c));
}

void printNodeData(NODE *node) {
	printf(" %*i ", NUM_DIGITS, node->data);
}

void preOrderPrint(NODE *node) {
	if (node == NULL) return;

	printNodeData(node);
	preOrderPrint(node->left);
	preOrderPrint(node->right);
}

void inOrderPrint(NODE *node) {
	if (node == NULL) return;

	inOrderPrint(node->left);
	printNodeData(node);
	inOrderPrint(node->right);
}

void postOrderPrint(NODE *node) {
	if (node == NULL) return;

	postOrderPrint(node->left);
	postOrderPrint(node->right);
	printNodeData(node);
}

char *prettyPrintTree(NODE *node, int *width, int *depth) {
	// base case
	if (node == NULL) {
		*width = 1;
		*depth = 0;

		// assign default value through malloc so it can be freed by its parent later
		char *treeString = malloc(sizeof(char) * 2);
		*treeString = ' ';
		*(treeString + 1) = 0;
		return treeString;
	}

	// initialize all variables
	int leftWidth = -1;
	int rightWidth = -1;
	int leftDepth = -1;
	int rightDepth = -1;

	// get the string representation of left and right subtrees
	char *leftString = prettyPrintTree(node->left, &leftWidth, &leftDepth);
	char *rightString = prettyPrintTree(node->right, &rightWidth, &rightDepth);

	// get the number of characters the node's value will be
	int nodeStringWidth = floor(log10(node->data)) + 1;

	// set the width and depth of the string based on the width and depth of the subtree strings
	*width = max(leftWidth + rightWidth + 1, nodeStringWidth);
	*depth = max(leftDepth, rightDepth) + 2;

	// allocate a rectangular space for (w + 1) x d + 1 characters.
	// (w + 1 to account for newlines, + 1 for termination)
	char *treeString = malloc(sizeof(char) * ( (*width + 1) * *depth + 1));

	// the first index to draw the node data
	int dataStartColumn = (*width - nodeStringWidth + 1) / 2;

	// allocate a string for the edges of the tree
	char *connectionLine = malloc(sizeof(char) * (*width + 1));

	// and populate it based on these rules:
	// - a '/' should be drawn above the left child
	// - a '^' schould be drawn below the node
	// - a '\' should be drawn above the right child
	// - in between the leftmost and rightmost of these three symbols should be '-'
	// - everything else should be filled with ' '
	// - if there are no children, nothing should be drawn
	//
	// EXAMPLES:
	// '  4  '
	// '/-^-\'
	// '5   9'
	//
	// this could occur with an extremely large left subtree
	// '    3    '
	// '    ^-/-\'
	// '      6 7'
	//
	// this could occur with an extremely large right subtree
	// '    3    '
	// '/-\-^    '
	// '6 7      '

	// add termination
	*(connectionLine + *width) = 0;

	int leftConnection = (leftDepth == 0) ? -1 : leftWidth / 2;
	int selfConnection = dataStartColumn + (nodeStringWidth - 1) / 2;
	int rightConnection = (rightDepth == 0) ? -1 : leftWidth + 1 + rightWidth / 2;
	for (int i = 0; i < *width; i++) {
		char c = ' '; // print a space by default
		if (leftDepth != 0 || rightDepth != 0) {
			// print the appropriate characters at their connections
			if (i == leftConnection) c = '/';
			else if (i == selfConnection) c = '^';
			else if (i == rightConnection) c = '\\';
			// if i is left of the leftmost connection, print a ' '
			else if (i < min3(leftConnection, selfConnection, rightConnection)) c = ' ';
			// account for -1 being the default value, so the min will be -1 if any are missing
			else if (min3(leftConnection, selfConnection, rightConnection) == -1) {
				if (i < (leftDepth == -1 ? min(selfConnection, rightConnection) : min(selfConnection, leftConnection)))
					c = ' ';
			}
			// and print a '-' if it's still in the line and nothing else has happened
			else if (i < max3(leftConnection, selfConnection, rightConnection)) c = '-';
		}

		*(connectionLine + i) = c;
	}
	// print the node data with correct padding, then the connectionLine
	sprintf(treeString, "%*s%i%*s\n%s\n", dataStartColumn, "", node->data, *width - (dataStartColumn + nodeStringWidth), "", connectionLine);

	free(connectionLine);
	connectionLine = NULL;

	// nor draw the subtrees next to each other
	for (int row = 2; row < *depth; row++) {
		// allocate each line, including newline and termination
		char *line = malloc(*width + 2);
		// fill it with spaces
		sprintf(line, "%*s\n", *width, "");

		// the the row exists in the left subtree, insert its string into the current row (minus the newlline)
		if (row - 2 < leftDepth)
			for (int i = 0; i < leftWidth; i++)
				*(line + i) = *(leftString + (row - 2) * (leftWidth + 1) + i);
		
		// the the row exists in the right subtree, insert its string into the current row (minus the newlline)
		if (row - 2 < rightDepth)
			for (int i = 0; i < rightWidth; i++)
				*(line + leftWidth + 1 + i) = *(rightString + (row - 2) * (rightWidth + 1) + i);
		
		// copy the line into the tree string, without termination character
		// (i tried this with both strcpy and memcpy and neither worked especially well)
		for (int i = 0; i <= *width; i++)
			*(treeString + row * (*width + 1) + i) = *(line + i);
	}

	// free the subtrees
	free(leftString);
	leftString = NULL;
	free(rightString);
	rightString = NULL;
	
	return treeString;
}

NODE *treeFromInOrderPreOrder(int *inorder, int *preorder, int numElements) {
	if (numElements == 0) return NULL;

	NODE *newNode = malloc(sizeof(NODE));
	newNode->data = *preorder;
	newNode->left = NULL;
	newNode->right = NULL;

	if (numElements == 1) return newNode;

	int i = 0;
	while(*preorder != *(inorder + i)) i++;

	int *leftPreOrder = malloc(i * sizeof(int));
	int next = 0;
	for (int k = 0; k < numElements; k++) {
		for (int j = 0; j < i; j++) {
			if (*(preorder+k) == *(inorder+j)) {
				*(leftPreOrder+next++) = *(preorder+k);
				break;
			}
		}
	}

	newNode->left = treeFromInOrderPreOrder(inorder, leftPreOrder, i);
	free(leftPreOrder);
	leftPreOrder = NULL;

	int *rightPreOrder = malloc((numElements - i - 1) * sizeof(int));
	next = 0;
	for (int k = 0; k < numElements; k++) {
		for (int j = i + 1; j < numElements; j++) {
			if (*(preorder+k) == *(inorder+j)) {
				*(rightPreOrder+next++) = *(preorder+k);
				break;
			}
		}
	}

	newNode->right = treeFromInOrderPreOrder(inorder + i + 1, rightPreOrder, (numElements - i - 1));
	free(rightPreOrder);
	rightPreOrder = NULL;

	return newNode;
}

void deleteTree(NODE *head) {
	if (head == NULL) return;
	deleteTree(head->left);
	deleteTree(head->right);
	free(head);
}

int *arrayFromLine(char *line, int *size) {
	int numberLength = 0;
	int arraySize = 0;
	char numberBuffer[24];
	for (int i = 0; i < strlen(line); i++) {
		if (*(line + i) >= 48 && *(line + i) <= 57) // it is a digit 0-9
		numberLength++;
			// numberBuffer[numberLength++] = *(line + i);
		else {
			if (numberLength == 0)
				continue;
			// numberBuffer[numberLength] = 0;
			numberLength = 0;
			arraySize++;
		}
	}

	int *array = malloc(sizeof(int) * arraySize);
	arraySize = 0;

	for (int i = 0; i < strlen(line); i++) {
		if (*(line + i) >= 48 && *(line + i) <= 57) // it is a digit 0-9
			numberBuffer[numberLength++] = *(line + i);
		else {
			if (numberLength == 0)
				continue;
			numberBuffer[numberLength] = 0;
			numberLength = 0;
			*(array + arraySize++) = atoi(numberBuffer);
		}
	}

	*size = arraySize;
	return array;
}

int main(int argc, char *argv[]) {
	if (argc <= 1)
		return 1;

	// srand(time(0));
	srand(1);
	int *inorder;
	int *preorder;
	int numElements = 0;

	char buffer[1024];
	NODE *tree = NULL;

	FILE *treeFile = fopen(argv[1], "r");
	int i = 0;
	while (fgets(buffer, sizeof(buffer), treeFile)) {
		if (i++ % 2) { // preorder
			preorder = arrayFromLine(buffer, &numElements);

			tree = treeFromInOrderPreOrder(inorder, preorder, numElements);
			
			int treeWidth = 0;
			int treeHeight = 0;

			printf("%s\n", prettyPrintTree(tree, &treeWidth, &treeHeight));

			deleteTree(tree);
			tree = NULL;

			free(inorder);
			inorder = NULL;

			free(preorder);
			preorder = NULL;

		} else { // inorder
			inorder = arrayFromLine(buffer, &numElements);
		}
	}
}