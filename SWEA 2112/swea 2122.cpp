#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
#define A 0
#define B 1

int T, D, W, K, mininput;
int drug[21][14]; //순서 바꿈 W * D
int drugcopy[21][14];

void set_line(int idx, int alphabet);

void init() {
	memset(drug, 0, sizeof(drug));
	memset(drugcopy, 0, sizeof(drugcopy));
	cin >> D >> W >> K;
	mininput = K;


	for (int i = 0; i < D; ++i) {
		for (int j = 0; j < W; ++j) {
			cin >> drug[j][i];
			drugcopy[j][i] = drug[j][i];
		}
	}
}

bool check_drug() {
	int maxres, temp;
	for (int i = 0; i < W; ++i) {
		maxres = 0; temp = 1;
		for (int j = 1; j < D; ++j)
		{
			if (drug[i][j - 1] == drug[i][j]) {
				// 전과 같으면 더해주고
				++temp;
				maxres = max(temp, maxres);
				if (maxres == K) break;
			}
			else { //다르면 1로 초기화
				temp = 1;
			}
		}
		if (maxres < K)
			return false;
	}

	return true;
}

void set_line(int idx, int alphabet) {
	// drug[x][idx] 번째 
	for (int i = 0; i < W; ++i)
		drug[i][idx] = alphabet;
}

void erase_line(int idx) {
	for (int i = 0; i < W; ++i)
		drug[i][idx] = drugcopy[i][idx];
}

void dfs(int curidx, int curinput) {

	// k보다 크면 종료
	if (curinput >= mininput)
		return;

	// 만족하게 되면 종료
	bool isK = check_drug();
	if (isK) {
		mininput = min(mininput, curinput);
		return;
	}

	if (curidx >= D)
		return;


	// 1. A로 통일
	set_line(curidx, A);
	dfs(curidx + 1, curinput + 1);
	erase_line(curidx);

	//2. B로 통일
	set_line(curidx, B);
	dfs(curidx + 1, curinput + 1);
	erase_line(curidx);

	//3. 안바꾸고 넘어감
	dfs(curidx + 1, curinput);

}

void solve() {
	if (K == 1) {
		mininput = 0;
		return;
	}
	// dfs : idx / 현재투입 
	dfs(0, 0);
}

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0);

	cin >> T;
	for (int t = 1; t <= T; ++t) {
		init();
		solve();
		cout << "#" << t << " " << mininput << "\n";
	}

	return 0;
}