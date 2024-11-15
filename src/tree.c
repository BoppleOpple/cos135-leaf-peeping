
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

/*
 * +
 * |
 * |
 * |
 * |
 * |
 * |+--+ 
 * |1 345
 * +
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
**/
char *prettyPrintTree(NODE *node, int *width, int *depth) {
	if (node == NULL) {
		*width = 1;
		*depth = 0;
		return " ";
	}
	int leftWidth = -1;
	int rightWidth = -1;
	int leftDepth = -1;
	int rightDepth = -1;
	
	char *leftString = prettyPrintTree(node->left, &leftWidth, &leftDepth);
	char *rightString = prettyPrintTree(node->right, &rightWidth, &rightDepth);

	int nodeStringWidth = floor(log10(node->data)) + 1;

	*width = max(leftWidth + rightWidth + 1, nodeStringWidth);
	*depth = max(leftDepth, rightDepth) + 2;

	char *treeString = malloc(sizeof(char) * ( (*width + 1) * (*depth + 2) + 1));

	int dataStartColumn = (*width - nodeStringWidth + 1) / 2;

	char *connectionLine = malloc(sizeof(char) * (*width + 1));

	*(connectionLine + *width) = 0;
	for (int i = 0; i < *width; i++) {
		char c;
		if (i < leftWidth / 2 || (i < dataStartColumn && leftDepth == 0)) c = ' ';
		else if (i == leftWidth / 2) c = '/';
		else if (i == dataStartColumn && (leftDepth != 0 || rightDepth != 0)) c = '^';
		else if (i < leftWidth + 1 + rightWidth / 2 && (leftDepth != 0 || rightDepth != 0)) c = '-';
		else if (i == leftWidth + 1 + rightWidth / 2 && rightDepth != 0) c = '\\';
		else c = ' ';

		*(connectionLine + i) = c;
	}

	sprintf(treeString, "%*s%i%*s\n%s\n", dataStartColumn, "", node->data, *width - (dataStartColumn + nodeStringWidth), "", connectionLine);

	
	for (int row = 2; row < *depth; row++) {
		char *line = malloc(*width + 2);
		sprintf(line, "%*s\n", *width, "");

		if (row - 2 < leftDepth)
			for (int i = 0; i < leftWidth; i++)
				*(line + i) = *(leftString + (row - 2) * (leftWidth + 1) + i);
		
		if (row - 2 < rightDepth)
			for (int i = 0; i < rightWidth; i++)
				*(line + leftWidth + 1 + i) = *(rightString + (row - 2) * (rightWidth + 1) + i);
		
		for (int i = 0; i <= *width; i++)
			*(treeString + row * (*width + 1) + i) = *(line + i);
	}
	
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
			// printf("preorder:  [");
			// preOrderPrint(tree);
			// printf("]\n");

			// printf("inorder:   [");
			// inOrderPrint(tree);
			// printf("]\n");

			// printf("postorder: [");
			// postOrderPrint(tree);
			// printf("]\n\n");

			deleteTree(tree);
			tree = NULL;

			free(inorder);
			inorder = NULL;

			free(preorder);
			preorder = NULL;

			// return 0;
		} else {
			inorder = arrayFromLine(buffer, &numElements);
		}
	}
}