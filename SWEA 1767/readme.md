실행시간 : 10ms

1. R, L, D, U 로 나누어 전선을 까는 길에 core가 있으면 제외
2. dfs 로 모든 경우의 수 탐색
3. 현재 core수 + 남은 core수 < 이제까지 나온 max core수이면 가지치기
