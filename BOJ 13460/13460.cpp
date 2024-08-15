#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;
#define L 0
#define R 1
#define U 2
#define D 3
#define Red 5
#define Blue 10
#define H 100

typedef pair<int, int> pii;

typedef struct information {
	int ord=0, direc=0;
	pii aloc, bloc;
	information(int _ord, int _direc, pii _aloc, pii _bloc) {
		ord = _ord;
		direc = _direc;
		aloc = _aloc;
		bloc = _bloc;
	}
}Info;

int N, M; // 새로 가로
int map[11][11];
int mintry = 11;
pii loc_a, loc_b, loc_h;

void init() {
	cin >> N >> M;
	char buf[20];
	for (int i = 0; i < N; ++i) {
		cin >> buf;
		for (int j = 0; j < M; ++j) {
			if (buf[j] == '#')
				map[i][j] = 1;
			else if (buf[j] == '.')
				map[i][j] = 0;
			else if (buf[j] == 'R') {
				map[i][j] = 0;
				loc_a = { i,j };
			}
			else if (buf[j] == 'B') {
				map[i][j] = 0;
				loc_b = { i,j };
			}
			else {
				map[i][j] = H;
				loc_h = { i,j };
			}	
		}
	}

}

int move(int direc) {

	bool movedA = true, movedB = true;
	bool holeA = false, holeB = false;
	int a_cnt = 0, b_cnt = 0;
	int ai, aj, bi, bj;
	ai = loc_a.first; aj = loc_a.second; bi = loc_b.first; bj = loc_b.second;

	int movei, movej;
	if (direc == L) {
		movei = 0; movej = -1;
	}
	else if (direc == R) {
		movei = 0; movej = 1;
	}
	else if (direc == U) {
		movei = -1; movej = 0;
	}
	else {
		movei = 1; movej = 0;
	}

	// 움직임
	while (movedA || movedB) {

		//cout << "현재 A 위치 : " << ai << ", " << aj << "  현재 B 위치 : " << bi << ", " << bj << "  방향 : " << direc << endl;
		//cout << "움직인 A 위치 : " << ai+movei << ", " << aj+movej << "  움직인 B 위치 : " << bi+movei << ", " << bj+movej << "  방향 : " << direc << endl;

		if (!holeA) {
			if (map[ai + movei][aj + movej] == 0) {
				if (ai + movei == bi && aj + movej == bj) {	// B에 부딪힌 경우
					movedA = false;
				}
				else {	//움직일 수 있는 빈공간에 갔을 경우
					ai = ai + movei; aj = aj + movej;
					movedA = true; ++a_cnt;
				}
			}
			else {
				if (map[ai + movei][aj + movej] == H) {	// 구멍에 부딪힌 경우
					holeA = true; movedA = false; ++a_cnt; ai = -1; aj = -1;
				}
				else {	// 벽에 부딪힌 경우
					movedA = false; 
				}
			}

		}

		if (!holeB) {
			if (map[bi + movei][bj + movej] == 0) {
				if (bi + movei == ai && bj + movej == aj) {	//A에 부딪힌 경우
					movedB = false;
				}
				else {	// 움직일 수 있는 빈공간으로 이동
					bi = bi + movei; bj = bj + movej;
					movedB = true; ++b_cnt;
				}
			}
			else {
				if (map[bi + movei][bj + movej] == H) {	// 구멍에 부딪힌 경우
					holeB = true; movedB = false; ++b_cnt;
					map[bi][bj] = 0;
					bi = -1; bj = -1;
				}
				else {	// 벽에 부딪힌 경우
					movedB = false;
				}
			}
		}

	}

	loc_a = { ai, aj };
	loc_b = { bi, bj };

	if (a_cnt == 0 && b_cnt == 0){
		// 아예 움직일 수 없었을 때
		return 1;
	}
	else if (holeB) {
		// B에 구멍이 들어갔을 때
		return 2;
	}
	else if (holeA) {
		// A에만 구멍이 들어갔을 때
		return 3;
	}
	else {
		// 다 움직인 후 둘다 구멍에 들어가지 않았을 때
		return 4;
	}

}

void bfs() {

	//1 - 아예 움직일 수 없었을 때 - 실패, 종료
	//2 - B에 구멍이 들어갔을 때 - 실패, 종료
	//3 - A에만 구멍이 들어갔을 때 - 성공!
	//4 - 다 움직였지만 둘 다 구멍에 들어가지 않았을 때 - Next!
	// information(int _ord, int _direc, pii _aloc, pii _bloc)
	queue<Info> q;

	q.push(Info(1, L, loc_a, loc_b));
	q.push(Info(1, R, loc_a, loc_b));
	q.push(Info(1, U, loc_a, loc_b));
	q.push(Info(1, D, loc_a, loc_b));

	while (!q.empty()) {

		Info temp = q.front();
		q.pop();

		if (temp.ord >= mintry)
			continue;


		loc_a = temp.aloc; loc_b = temp.bloc;
		int res = move(temp.direc);
		if (res == 1) {
			// 아예 움직일 수 없었을 때
			continue;
		}
		else if (res == 2) {
			// B에 구멍이 들어갔을 때
			continue;
		}
		else if (res == 3) {
			// A에만 구멍이 들어갔을 때
			mintry = min(mintry, temp.ord);
		}
		else {
			// 다 움직인 후 둘다 구멍에 들어가지 않았을 때
			temp.aloc = loc_a; temp.bloc = loc_b;
			q.push(Info(temp.ord + 1, L, temp.aloc, temp.bloc));
			q.push(Info(temp.ord + 1, R, temp.aloc, temp.bloc));
			q.push(Info(temp.ord + 1, U, temp.aloc, temp.bloc));
			q.push(Info(temp.ord + 1, D, temp.aloc, temp.bloc));
		}
	}

}

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0);

	init();
	bfs();
	if (mintry == 11) {
		cout << -1;
	}
	else {
		cout << mintry;
	}

	return 0;
}