#include <iostream>

using namespace std;

#define LOG(x) cout << x << endl;

const float EPSILON = 0.0001f;

void PrintMatrix (float** matrix, int degree) {
	if (matrix == nullptr)
		return;

	for (int row = 0; row < degree; ++row) {
		for (int col = 0; col < degree; ++col) {
			cout << matrix[row][col];
			if (col != degree - 1) {
				cout << "\t";
			}
		}
		cout << endl;
	}
}

void PrintMatrix (float** matrix, int rowDegree, int colDegree) {
	if (matrix == nullptr)
		return;

	for (int row = 0; row < rowDegree; ++row) {
		for (int col = 0; col < colDegree; ++col) {
			cout << matrix[row][col];
			if (col != colDegree - 1) {
				cout << "\t";
			}
		}
		cout << endl;
	}
}

float** MatrixSet (int degree) {
	float** matrix = new float* [degree];
	for (int row = 0; row < degree; ++row) {
		matrix[row] = new float[degree];
	}

	for (int row = 0; row < degree; ++row) {
		for (int col = 0; col < degree; ++col) {
			matrix[row][col] = 0;
		}
	}

	return matrix;
}

float** MatrixSet (const float* values, int degree) {
	float** matrix = new float* [degree];
	for (int row = 0; row < degree; ++row) {
		matrix[row] = new float[degree];
	}

	for (int row = 0; row < degree; ++row) {
		for (int col = 0; col < degree; ++col) {
			matrix[row][col] = values[row * degree + col];
		}
	}

	return matrix;
}

float** MatrixSet (int rowDegree, int colDegree) {
	float** matrix = new float* [rowDegree];
	for (int row = 0; row < rowDegree; ++row) {
		matrix[row] = new float[colDegree];
	}

	for (int row = 0; row < rowDegree; ++row) {
		for (int col = 0; col < colDegree; ++col) {
			matrix[row][col] = 0;
		}
	}

	return matrix;
}

float** MatrixSet (const float* values, int rowDegree, int colDegree) {
	float** matrix = new float* [rowDegree];
	for (int row = 0; row < rowDegree; ++row) {
		matrix[row] = new float[colDegree];
	}

	for (int row = 0; row < rowDegree; ++row) {
		for (int col = 0; col < colDegree; ++col) {
			matrix[row][col] = values[row * colDegree + col];
		}
	}

	return matrix;
}

/// <summary>
/// 計算轉置矩陣
/// </summary>
float** CalculateTranspose (float** matrix, int degree) {
	float** result = MatrixSet (degree);
	for (int i = 0; i < degree; ++i) {
		for (int j = 0; j < degree; ++j) {
			result[j][i] = matrix[i][j];
		}
	}
	return result;
}

/// <summary>
/// 計算余子陣, 從(rowIndex, colIndex)展開
/// </summary>
float** CalculateMinor (float** pMatrix, int degree, int rowIndex, int colIndex) {
	float** newMatrix = MatrixSet (degree);

	for (int row = 0; row < degree - 1; ++row) {
		for (int col = 0; col < degree - 1; ++col) {
			int temp_row = row, temp_col = col;
			if (row >= rowIndex) {
				temp_row++;
			}
			if (col >= colIndex) {
				temp_col++;
			}

			newMatrix[row][col] = pMatrix[temp_row][temp_col];
		}
	}

	return newMatrix;
}

/// <summary>
/// 得到最多0的行
/// </summary>
int* GetMaxZeroCountRow (float** matrix, int degree) {
	// result[0]: row index, result[1]: max count
	int* result = new int[2] {0};
	for (int row = 0; row < degree; ++row) {
		int temp = 0;
		for (int col = 0; col < degree; ++col) {
			if (matrix[row][col] == 0) {
				temp++;
			}
		}

		if (temp > result[1]) {
			result[0] = row;
			result[1] = temp;

		}
	}
	return result;
}

/// <summary>
/// 得到最多0的列
/// </summary>
int* GetMaxZeroCountCol (float** matrix, int degree) {
	// result[0]: row index, result[1]: max count
	int* result = new int[2] {0};
	for (int col = 0; col < degree; ++col) {
		int temp = 0;
		for (int row = 0; row < degree; ++row) {
			if (matrix[row][col] == 0) {
				temp++;
			}
		}

		if (temp > result[1]) {
			result[0] = col;
			result[1] = temp;

		}
	}
	return result;
}

/// <summary>
/// 計算行列式
/// </summary>
float CalculateDeterminant (float** matrix, int degree) {
	// [a b]
	// [c d]
	// formula: ad - bc
	if (degree == 2) {
		float det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		return det;
	} else if (degree == 1) {
		return matrix[0][0];
	}

	// 1. 獲得最多0的行或列
	// 2. 按該行/列展開
	// 3. 計算行列式
	int* rowInf = GetMaxZeroCountRow (matrix, degree);
	cout << "RowIndex: " << rowInf[0] << ", ZeroCount: " << rowInf[1] << endl;
	int* colInf = GetMaxZeroCountCol (matrix, degree);
	cout << "ColumnIndex: " << colInf[0] << ", ZeroCount: " << colInf[1] << endl << endl;

	float result = 0;
	if (colInf[1] > rowInf[1]) {
		// 選擇按列展開
		for (int rowIndex = 0; rowIndex < degree; rowIndex++) {
			int colIndex = colInf[0];
			float element = matrix[rowIndex][colIndex];
			if (element == 0) {
				// continue;
			}

			cout << "[Minor " << rowIndex + 1 << " by (" << rowIndex << ", " << colIndex << ")]" << endl;
			float** minor = CalculateMinor (matrix, degree, rowIndex, colIndex);
			PrintMatrix (minor, degree - 1);

			float sign = -1;
			if ((rowIndex + colIndex) % 2 == 0)
				sign = 1;

			float det = sign * element * CalculateDeterminant (minor, degree - 1);
			cout << "Determinant: " << det << endl << endl;
			result += det;
		}
	} else {
		// 選擇按行展開
		for (int colIndex = 0; colIndex < degree; colIndex++) {
			int rowIndex = rowInf[0];
			float element = matrix[rowIndex][colIndex];
			if (element == 0) {
				// continue;
			}

			cout << "[Minor " << colIndex + 1 << " by (" << rowIndex << ", " << colIndex << ")]" << endl;
			float** minor = CalculateMinor (matrix, degree, rowIndex, colIndex);
			PrintMatrix (minor, degree - 1);

			float sign = -1;
			if ((rowIndex + colIndex) % 2 == 0)
				sign = 1;

			float det = sign * element * CalculateDeterminant (minor, degree - 1);
			cout << "Determinant: " << det << endl << endl;
			result += det;
		}
	}

	return result;
}

/// <summary>
/// 計算伴隨矩陣
/// </summary>
float** CalculateAdjoint (float** matrix, int degree) {
	float** result = MatrixSet (degree);
	for (int row = 0; row < degree; ++row) {
		for (int col = 0; col < degree; ++col) {
			// 1. 得到余子陣
			// 2. 計算余子陣的行列式
			float sign = -1;
			if ((row + col) % 2 == 0)
				sign = 1;

			float** minor = CalculateMinor (matrix, degree, row, col);
			result[row][col] = sign * CalculateDeterminant (minor, degree - 1);
		}
	}

	// 最後轉置
	return CalculateTranspose (result, degree);
}

/// <summary>
/// 計算逆矩陣
/// </summary>
float** CalculateInverse (float** matrix, int degree) {
	float det = CalculateDeterminant (matrix, degree);
	cout << "Final Determinant: " << det << endl;
	if (abs (det) - EPSILON <= 0) {
		cout << "行列式為0，不存在逆矩陣。" << endl;
		return nullptr;
	}

	float** result = CalculateAdjoint (matrix, degree);

	for (int row = 0; row < degree; ++row) {
		for (int col = 0; col < degree; ++col) {
			result[row][col] = result[row][col] / det;
		}
	}

	return result;
}

float** MatrixAdd (float** matrix, float** matrix2, int rowDegree, int colDegree) {
	float** result = MatrixSet (rowDegree, colDegree);
	for (int row = 0; row < rowDegree; ++row) {
		for (int col = 0; col < colDegree; ++col) {
			result[row][col] = matrix[row][col] + matrix2[row][col];
		}
	}

	return result;
}

float** MatrixMinus (float** matrix, float** matrix2, int rowDegree, int colDegree) {
	float** result = MatrixSet (rowDegree, colDegree);
	for (int row = 0; row < rowDegree; ++row) {
		for (int col = 0; col < colDegree; ++col) {
			result[row][col] = matrix[row][col] - matrix2[row][col];
		}
	}

	return result;
}

float* GetColumnElements (float** matrix, int rowDegree, int colDegree, int colIndex) {
	if (colIndex > colDegree)
		return NULL;

	float* result = new float[rowDegree];
	for (int row = 0; row < rowDegree; row++) {
		result[row] = matrix[row][colIndex];
	}

	return result;
}

float** MatrixMultiply (float** matrix, float** matrix2, int matrix1RowDegree, int matrix1ColDegree, int matrix2RowDegree, int matrix2ColDegree) {
	// 當矩陣A的列數與矩陣B的行數不符時，無法相乘
	if (matrix1ColDegree != matrix2RowDegree) {
		LOG ("矩陣A的列數與矩陣B的行數不符，無法相乘。");
		return nullptr;
	}

	float** result = MatrixSet (matrix1RowDegree, matrix2ColDegree);
	// 利用列向量做乘法
	for (int i = 0; i < matrix1RowDegree; ++i) {
		float* vec = new float[matrix2ColDegree];
		float* matrixRow = matrix[i];

		for (int j = 0; j < matrix2ColDegree; ++j) {
			float* matrix2Col = GetColumnElements (matrix2, matrix2RowDegree, matrix2ColDegree, j);
			float element = 0.0f;
			for (int k = 0; k < matrix1ColDegree; ++k) {
				float matrix1Element = matrixRow[k];
				float matrix2Elewment = matrix2Col[k];
				element += matrix1Element * matrix2Elewment;
			}
			vec[j] = element;
		}

		result[i] = vec;
	}

	return result;
}

int main () {
	//float* values = new float[] {1.0f, 0.0f, 0.0f, 0.0f,
	//	0.0f, 2.0f, 0.0f, 0.0f,
	//	0.0f, 0.0f, 4.0f, 0.0f,
	//	1.0f, 2.0f, 3.0f, 1.0f};

	//float* values = new float[] {1, 2, 
	//	3, 4};

	//float* values = new float[] {1, 2, 3,
	//	4, 5, 6,
	//	7, 8, 9};

	float* values = new float[] {1, 4, 8, 7,
		6, 5, 7, 8,
		1, 5, 7, 5,
		3, 5, 4, 7};

	float* values2 = new float[] {1, 4, 8, 9,
		0, 1, 5, 7,
		0, 2, 7, 8,
		3, 4, 8, 7};

	//float* values = new float[] {2, 3, 4,
	//	5, 6, 7};
	//float* values2 = new float[] {1, 2, 3,
	//	5, 1, 2};
	//float* values3 = new float[] {1, 2,
	//	3, 4,
	//	5, 7};


	float** matrix = MatrixSet (values, 4);
	float** matrix2 = MatrixSet (values2, 4);
	// float** matrix3 = MatrixSet (values3, 3, 2);

	LOG ("[ADD]");
	float** add = MatrixAdd (matrix, matrix2, 4, 4);
	PrintMatrix (add, 4);
	LOG ("");

	LOG ("[MINUS]");
	float** minus = MatrixMinus (matrix, matrix2, 4, 4);
	PrintMatrix (minus, 4);
	LOG ("");

	LOG ("[MULTIPLY]");
	float** multiply = MatrixMultiply (matrix, matrix2, 4, 4, 4, 4);
	PrintMatrix (multiply, 4);
	LOG ("");
}