#include <iostream>
#include <cstring>
#include <vector>
using namespace std;
#define R 0
#define L 1
#define U 2
#define D 3
typedef pair<int, int> pii;

int T, N, maxscore;
int board[101][101];	// ���� ����
vector<pii> whole[5];	// ��Ȧ��
vector<pii> startdot;	// ������
int block[5][4] = {
	//R L  U  D
	{L, U, D, R},
	{L, D, R, U},
	{D, R, L, U},
	{U, R, D, L},
	{L, R, D, U}
};
int wall[4] = { L, R, D, U };
int dmove[4][2] = {
	{0, 1},		//R
	{0, -1},	//L
	{-1, 0},	//U
	{1, 0}		//D
};

void init() {

	memset(board, 0, sizeof(board));
	for (int i = 0; i < 5; ++i)
		whole[i].clear();
	startdot.clear();
	maxscore = 0;


	cin >> N;
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			cin >> board[i][j];
			if (board[i][j] >= 6) {
				whole[board[i][j] - 6].push_back({ i,j });
			}
			if (board[i][j] == 0) {
				startdot.push_back({ i,j });
			}
		}
	}
}

int iswall(int i, int j, int direc) {
	if (direc == R) {
		if (j >= N)
			return wall[R];
	}
	else if (direc == L) {
		if (j < 0)
			return wall[L];
	}
	else if (direc == U) {
		if (i < 0)
			return wall[U];
	}
	else {
		if (i >= N)
			return wall[D];
	}
	return -1;
}

void dfs(int starti, int startj, int direc, int tempscore) {
	// �ʱ���ġ�� ���ƿ԰ų� ��Ȧ�� �����ϸ� ���� ������ �ִ� ���� ���ؼ� �����ϰ� ������
	// �� ������ ��
	// ���� ������ ��
	// ��Ȧ ������ ��
	int tempi = starti, tempj = startj;
	tempi += dmove[direc][0]; tempj += dmove[direc][1];

	while (1) {

		//cout << " i, j" << tempi <<" "<< tempj << endl;

		if ((tempi == starti && tempj == startj) || board[tempi][tempj] == -1) {
			//cout << "�̹� �б� ����" << endl;
			maxscore = max(maxscore, tempscore);
			break;
		}

		// 1. ������ üũ 
		int newdirec = iswall(tempi, tempj, direc);

		if (newdirec >= 0) {
			//cout<<"������"<<endl;
			direc = newdirec;
			++tempscore;
		}
		else if (board[tempi][tempj] >= 1 && board[tempi][tempj] <= 5) {
			// 2. ������ üũ
			//cout<<"������"<<endl;
			++tempscore;
			int blocktype = board[tempi][tempj] - 1;
			int tempdirec = direc;
			direc = block[blocktype][tempdirec];
		}
		else if (board[tempi][tempj] >= 6 && board[tempi][tempj]<=10) {
			// 3. ��Ȧ���� üũ
			//cout << "��Ȧ ����" << endl;
			int holetype = board[tempi][tempj] - 6;

			for (int i = 0; i < whole[holetype].size(); ++i) 
			{
				int curi = whole[holetype][i].first;
				int curj = whole[holetype][i].second;
				if (curi == tempi && curj == tempj) {
					continue;
				}
				else {
					tempi = curi; tempj = curj;
					break;
				}
			}

		}
		
		tempi += dmove[direc][0]; 
		tempj += dmove[direc][1];

	}

}

void solve() {
	// startdot���� ���� 
	// dfs : ���� ��ġ, ���� ����, ���� ����
	for (int i = 0; i < startdot.size(); ++i) {
		int tempi = startdot[i].first;
		int tempj = startdot[i].second;
		
		dfs(tempi, tempj, R, 0);
		dfs(tempi, tempj, L, 0); 
		dfs(tempi, tempj, U, 0);
		dfs(tempi, tempj, D, 0);
	}

}

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0);

	cin >> T;
	for (int t = 1; t <= T; ++t) {
		init();
		solve();
		cout << "#" << t << " " << maxscore << "\n";
	}

	return 0;
}