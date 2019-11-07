//
// Created by John on 12/06/2019.
//

#include "BinaryTree.h"


BinaryTree::BinaryTree(QString &expression) {
	pRoot = new Node(expression);
}

BinaryTree::BinaryTree() = default;


void BinaryTree::setPRoot(QString &expression) {
	pRoot = new Node(expression);
}

double BinaryTree::calculateTree(double &xPlug) {
	// todo: rewrite this function
	// todo: instead of string x use struct
	// returns f(x)
	Node *currentNode = pRoot;

	bool returning = false;
	bool hasBeenLeft = false;

	// append 1 if moved to left or 2 if moved to right
	// 0 is only a placeholder to avoid empty list
	QList<int> movement = {0};

	while (true) {
		if (currentNode->pLeftChild && !returning) {
			movement.append(1);
			currentNode = currentNode->pLeftChild;
		} else if (currentNode->pRightChild && !returning) {
			movement.append(2);
			currentNode = currentNode->pRightChild;
		}

		// if node has no children
		if (!currentNode->pLeftChild && !currentNode->pRightChild) {
			returning = true;
		}

		if (returning) {
			if (currentNode->pParent) {
				currentNode = currentNode->pParent;
			}
			//50+(x+1)*(x+1)+10
			// try to go right
			if (movement.last() == 1) {
				movement.removeLast();

				if (currentNode->pRightChild) {
					returning = false;
					movement.append(2);
					currentNode = currentNode->pRightChild;
				} else {
					currentNode->doubleValue = computeOperation(currentNode, xPlug);
				}

			} else if (movement.last() == 2 && currentNode->pParent) {
				movement.removeLast();
				currentNode->doubleValue = computeOperation(currentNode, xPlug);
			}
		}

		if (currentNode == pRoot && returning) {
			break;
		}

	}
	currentNode->doubleValue = computeOperation(currentNode, xPlug);

	return currentNode->doubleValue;
}


QVector<double> BinaryTree::calculateTree(QVector<double> &xArray) {
	QVector<double> yArray(xArray.length());

	bool firstPass = false;
	for (int i = 0; i < xArray.length(); ++i) {
		yArray[i] = calculateTree(xArray[i]);
		// tixme: the following code snippet has performance issues
//		if (firstPass && abs(yArray[i] - yArray[i - 1]) > 100) {
//			qDebug() << yArray[i - 1] << -yArray[i];
//			yArray[i - 1] = _nan();
//			yArray[i] = _nan();
//		}
//		firstPass = true;
	}
	return yArray;
}


double BinaryTree::computeOperation(Node *node, double xPlug) {
	double doubleValueLeft = 0;
	double doubleValueRight = 0;

	// replace x with the number
	if (node->pLeftChild) {
		if (node->pLeftChild->strValue == "x") {
			doubleValueLeft = xPlug;
		} else {
			doubleValueLeft = node->pLeftChild->doubleValue;
		}
	}

	if (node->pRightChild) {
		if (node->pRightChild->strValue == "x") {
			doubleValueRight = xPlug;
		} else {
			doubleValueRight = node->pRightChild->doubleValue;
		}
	}

	if (node->mathOperation == "+") {
		return doubleValueLeft + doubleValueRight;
	} else if (node->mathOperation == "-") {
		return doubleValueLeft - doubleValueRight;
	} else if (node->mathOperation == "-") {
		return doubleValueLeft - doubleValueRight;
	} else if (node->mathOperation == "*") {
		return doubleValueLeft * doubleValueRight;
	} else if (node->mathOperation == "/") {
		return doubleValueLeft / doubleValueRight;
	} else if (node->mathOperation == "%") {
		if (int(doubleValueRight) != 0) {
			return int(doubleValueLeft) % int(doubleValueRight);
		}
		return _nan();
	} else if (node->mathOperation == "^") {
		// only change if the base is negative
		// ref: https://stackoverflow.com/a/8493081/10450514
		if (doubleValueLeft < 0) {
			// check whether the exponent is a fraction
			if (node->pRightChild->mathOperation == "/") {
				// It is not required to check whether doubleValue has been assigned
				// a value, because is will be already computed by the time the
				// program is here.
				double numerator = node->pRightChild->pLeftChild->doubleValue;
				double denominator = node->pRightChild->pRightChild->doubleValue;

				// ! all the even denominator roots have imaginary negative
				// ! while odd denominator roots have real negatives
				// check if denominator is an int + check if denominator is odd
				if (round(denominator) == denominator && int(denominator) % 2 != 0) {
					// if the denominator is odd, the negative values are real
					// if numerator is an int
					if (round(numerator) == numerator) {
						if (int(numerator) % 2 == 0) {
							// if numerator is even simply flip the
							// function above the y axis
							return qPow(-doubleValueLeft, doubleValueRight);
						} else {
							// if the numerator is odd, we need to calculate the points
							// by making them positive, because a negative base is not
							// possible.
							return -qPow(-doubleValueLeft, doubleValueRight);
						}
					}
				}
			}
		}
		return qPow(doubleValueLeft, doubleValueRight);
	} else if (node->mathOperation == "!") {
		// + 1 to match x!
		return tgamma(doubleValueLeft + 1);
	} else if (node->mathOperation == "sin") {
		return sin(doubleValueRight);
	} else if (node->mathOperation == "arcsin") {
		return asin(doubleValueRight);
	} else if (node->mathOperation == "cos") {
		return cos(doubleValueRight);
	} else if (node->mathOperation == "arccos") {
		return acos(doubleValueRight);
	} else if (node->mathOperation == "tan") {
		return tan(doubleValueRight);
	} else if (node->mathOperation == "sinh") {
		return sinh(doubleValueRight);
	} else if (node->mathOperation == "cosh") {
		return cosh(doubleValueRight);
	} else if (node->mathOperation == "tanh") {
		return tanh(doubleValueRight);
	} else if (node->mathOperation == "arcsinh") {
		return asinh(doubleValueRight);
	} else if (node->mathOperation == "arccosh") {
		return acosh(doubleValueRight);
	} else if (node->mathOperation == "arctanh") {
		return atanh(doubleValueRight);
	} else if (node->mathOperation == "arctan") {
		return atan(doubleValueRight);
	} else if (node->mathOperation == "log10") {
		return log10(doubleValueRight);
	} else if (node->mathOperation == "log") {
		return log10(doubleValueRight) / log10(node->logBase);
	} else if (node->mathOperation == "ln") {
		return qLn(doubleValueRight);
	}
	// node has no children, but has a value
	// functions that are just a number like 2
	// fixme: what if you just enter x?
	return node->doubleValue;

	throw std::logic_error("No math operation defined");
}


void BinaryTree::getNodeStats(Node *node) {
	qDebug() << "pParent: " << node->pParent << "\tpLeft_child: " << node->pLeftChild << "\tpRight_child: " << node->pRightChild;
	qDebug() << "operation: [" << node->pLeftChild->strValue << " (" << node->pLeftChild->doubleValue << ")] " << node->mathOperation << " ["
	         << node->pRightChild->strValue << " (" << node->pRightChild->doubleValue << ")]\n";
}

