#include <iostream>
#include <cstring>
#include <climits>
#include <vector>
#include <algorithm>
using namespace std;
#define R 0
#define L 1
#define D 2
#define U 3

int T, N, maxcores, minlines, tempmax, tempmaxlines;
int maxi[13][13];

typedef struct CORE {
	int x, y;
	bool l = true, r = true, d = true, u = true;
	CORE(int Y, int X) {
		x = X; y = Y;
	}
}CC;

vector<CC> cores;


void init() {

	maxcores = 0; minlines = 0; tempmax = 0; tempmaxlines = 0;
	cores.clear();
	memset(maxi, 0, sizeof(maxi));

	cin >> N;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> maxi[i][j];
			if (maxi[i][j] == 1)
			{
				if (i == 1 || i == N || j == 1 || j == N)
				{
					maxi[i][j] = 2;	// 전선 연결할 필요 없음 
					maxcores++;
				}
				else {
					cores.push_back(CORE(i, j));
				}
			}

		}
	}

}

void check_lines() {

	for (int k = 0; k < cores.size(); k++)
	{
		CC core = cores[k];
		int y = core.y;
		int x = core.x;
		// right : x : x+1 ~ N, y : y
		for (int i = x + 1; i <= N; i++) {
			if (maxi[y][i] > 0) {
				cores[k].r = false;
				break;
			}

		}

		// left : X : 1~ x-1, y : y
		for (int i = 1; i <= x - 1; i++) {
			if (maxi[y][i] > 0) {
				cores[k].l = false;
				break;
			}

		}

		// up : x : x, y : 1~ y-1
		for (int i = 1; i <= y - 1; i++) {
			if (maxi[i][x] > 0) {
				cores[k].u = false;
				break;
			}

		}

		// down : x : x, y : y+1 ~ N
		for (int i = y + 1; i <= N; i++) {
			if (maxi[i][x] > 0) {
				cores[k].d = false;
				break;
			}

		}

		//cout << "Y X : " << y << " " << x << " R L U D : " << cores[k].r << cores[k].l << cores[k].u << cores[k].d << endl;
	}

}

int check_direction(int lines[13][13], CC dot, int direc) {

	int y = dot.y;
	int x = dot.x;


	if (direc == R) {
		for (int i = x + 1; i <= N; i++) {
			if (lines[y][i] > 0)
				return 0;
		}
		return N - x;
	}
	else if (direc == L) {
		for (int i = 1; i <= x - 1; i++) {
			if (lines[y][i] > 0)
				return 0;
		}
		return x - 1;
	}
	else if (direc == D) {
		for (int i = y + 1; i <= N; i++) {
			if (lines[i][x] > 0)
				return 0;
		}
		return N - y;
	}
	else {
		for (int i = 1; i <= y - 1; i++) {
			if (lines[i][x] > 0)
				return 0;
		}
		return y - 1;
	}

}

void drawlines(int lines[13][13], CC dot, int direc) {
	int y = dot.y;
	int x = dot.x;

	if (direc == R) {
		for (int i = x + 1; i <= N; i++) {
			lines[y][i] = 1;
		}
	}
	else if (direc == L) {
		for (int i = 1; i <= x - 1; i++)
			lines[y][i] = 1;
	}
	else if (direc == D) {
		for (int i = y + 1; i <= N; i++)
			lines[i][x] = 1;
	}
	else {
		for (int i = 1; i <= y - 1; i++)
			lines[i][x] = 1;
	}
}

void eraselines(int lines[13][13], CC dot, int direc) {
	int y = dot.y;
	int x = dot.x;

	if (direc == R) {
		for (int i = x + 1; i <= N; i++) {
			lines[y][i] = 0;
		}
	}
	else if (direc == L) {
		for (int i = 1; i <= x - 1; i++)
			lines[y][i] = 0;
	}
	else if (direc == D) {
		for (int i = y + 1; i <= N; i++)
			lines[i][x] = 0;
	}
	else {
		for (int i = 1; i <= y - 1; i++)
			lines[i][x] = 0;
	}
}

void dfs(int temp_idx, int lines[13][13], int temp_cores, int temp_lines, int size) {

	if (temp_cores > tempmax) {
		tempmax = temp_cores;
		tempmaxlines = temp_lines;
	}
	else if (temp_cores == tempmax) {
		tempmaxlines = min(tempmaxlines, temp_lines);
	}

	if (temp_idx >= size) {
		return;
	}

	// 가지치기 : 이미 최적의 결과가 나온 경우
	if (temp_cores + (size - temp_idx) < tempmax) {
		return;
	}

	CC temp = cores[temp_idx];
	int y = temp.y, x = temp.x;
	int temp_l_length;

	bool tempb[4] = { temp.r, temp.l, temp.d, temp.u };

	// 1~4. R,L,D,U
	for (int i = 0; i < 4; i++) {
		if (tempb[i] == true) {
			temp_l_length = check_direction(lines, temp, i);
			if (temp_l_length > 0) {
				drawlines(lines, temp, i);
				dfs(temp_idx + 1, lines, temp_cores + 1, temp_lines + temp_l_length, size);
				eraselines(lines, temp, i);
			}
		}
	}

	// 5. 넣지않음
	dfs(temp_idx + 1, lines, temp_cores, temp_lines, size);

}

int solve() {
	// dfs로 최대 노드, 최소 거리 찾기
	// cores[0]부터 시작
	// 오른쪽, 왼쪽, 위, 아래, x
	// 필요 변수 : 현재idx, line이 담긴 배열(이전 line과 겹치면 못담으므로), 현재담은 코어개수, 현재 있는 라인 개수, 코어 갯수
	int lines[13][13] = { 0, };
	dfs(0, lines, 0, 0, cores.size());
	maxcores += tempmax;
	minlines = tempmaxlines;
	return minlines;
}

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0);

	cin >> T;
	int res;
	for (int t = 1; t <= T; t++) {
		init();
		check_lines();
		res = solve();

		cout << "#" << t << " " << res << "\n";
	}


	return 0;
}
