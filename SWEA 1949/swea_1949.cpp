#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
typedef pair<int, int> pii;
#define U 0
#define D 1
#define L 2
#define R 3


int map[10][10];
bool visited[10][10];
int T, K, N;
vector<pii> maxstart;
int maxroad;
pii direc[4] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

bool check(int x, int y) {
	if (x < 0 || x >= N || y < 0 || y >= N || visited[y][x] == true)
		return false;
	return true;
}

void init() {

	memset(map, 0, sizeof(map));
	memset(visited, 0, sizeof(visited));
	maxstart.clear();
	maxroad = 0;

	cin >> N >> K;

	int maxnum = 0;

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			cin >> map[i][j];
			maxnum = max(maxnum, map[i][j]);
		}
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			if (map[i][j] == maxnum)
				maxstart.push_back({ i,j });
		}
	}

}

void dfs(pii curidx, int curroad, bool kused) {

	int y = curidx.first, x = curidx.second;

	visited[y][x] = true;
	++curroad;

	if (curroad > maxroad)	//최대 길이 갱신
		maxroad = curroad;

	for (int i = 0; i < 4; i++) {
		int tempy = y + direc[i].second;
		int tempx = x + direc[i].first;

		if (check(tempx, tempy)) {
			if (map[tempy][tempx] < map[y][x]) {
				dfs({ tempy, tempx }, curroad, kused);
				visited[tempy][tempx] = false;
			}
			else if (!kused && map[tempy][tempx] - K < map[y][x]) {
				int tempk = map[tempy][tempx];
				map[tempy][tempx] = map[y][x] - 1;
				dfs({ tempy, tempx }, curroad, true);
				map[tempy][tempx] = tempk;
				visited[tempy][tempx] = false;
			}
		}

	}

}

int solve() {

	// dfs
	// 1. 높->낮
	// 2. 높->낮-K
	// bool usek, int startidx, int 현재load길이, bool visited(전역),
	for (int i = 0; i < maxstart.size(); i++) {
		memset(visited, 0, sizeof(visited));
		dfs(maxstart[i], 0, false);
	}

	return maxroad;
}

int main(int argc, char** argv) {
	ios_base::sync_with_stdio(0); cin.tie(0);

	cin >> T;
	int res = 0;
	for (int t = 0; t < T; ++t) {
		init();
		res = solve();
		cout << "#" << t + 1 << " " << res << "\n";
	}

	return 0;
}