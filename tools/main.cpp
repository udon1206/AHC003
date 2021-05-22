#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
#include <tuple>
#include <cmath>
#include <numeric>
#include <set>
#include <map>
#include <array>
#include <complex>
#include <iomanip>
#include <cassert>
#include <random>
#include <chrono>
#include <valarray>
#include <bitset>
using ll = long long;
using std::cin;
using std::cout;
using std::endl;
std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return 1; } return 0; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return 1; } return 0; }
const int inf = (int)1e9 + 7;
const long long INF = 1LL << 60;

std::tuple<int, int, int, int> input()
{
	int si, sj, ti, tj;
	cin >> si >> sj >> ti >> tj;
	return std::make_tuple(si, sj, ti, tj);
}
const int H = 30;
const int W = 30;
int ch[H][W];
int cw[H][W];
int chcount[H][W];
int cwcount[H][W];
inline int cost(int sh, int sw, int th, int tw)
{
	if(sh == th)
	{
		if(sw > tw)
			std::swap(sw, tw);
		return cw[sh][sw];
	}
	else
	{
		if(sh > th)
			std::swap(sh, sw);
		return ch[sh][sw];
	}
}

void updatech(int i, int j, int val)
{
	ll sum = 1LL * ch[i][j] * chcount[i][j] + 1LL * val * (1000 - chcount[i][j]) / 1000;
	chcount[i][j] += 1;
	ch[i][j] = sum / chcount[i][j];
}
void updatecw(int i, int j, int val)
{
	ll sum = 1LL * cw[i][j] * cwcount[i][j] + 1LL * val * (1000 - cwcount[i][j]) / 1000;
	cwcount[i][j] += 1;
	cw[i][j] = sum / cwcount[i][j];
}
void solve_first()
{
	auto [sh, sw, th, tw] = input();
	int curh = sh, curw = sw;
	if(sh == th)
	{
		int cnt = 0;
		std::string res;
		while(curw < tw)
		{
			res += 'R';
			curw += 1;
			cnt += 1;
		}
		while(curw > tw)
		{
			res += 'L';
			curw -= 1;
			cnt += 1;
		}
		cout << res << endl;
		ll len; cin >> len;
		len /= cnt;
		for (int i = 0; i < W - 1; ++i)
		{
			updatecw(th, i, len);
		}
	}
	else if(sw == tw)
	{
		int cnt = 0;
		std::string res;
		while(curh < th)
		{
			res += 'D';
			curh += 1;
			cnt += 1;
		}
		while(curh > th)
		{
			res += 'U';
			curh -= 1;
			cnt += 1;
		}
		cout << res << endl;
		ll len; cin >> len;
		len /= cnt;
		for (int i = 0; i < H - 1; ++i)
		{
			updatech(i, sw, len);
		}
	}
	else
	{
		int cnt = 0;
		std::string res;
		while(curh < th)
		{
			res += 'D';
			curh += 1;
			cnt += 1;
		}
		while(curh > th)
		{
			res += 'U';
			curh -= 1;
			cnt += 1;
		}
		while(curw < tw)
		{
			res += 'R';
			curw += 1;
			cnt += 1;
		}
		while(curw > tw)
		{
			res += 'L';
			curw -= 1;
			cnt += 1;
		}
		cout << res << endl;
		ll len; cin >> len;
		len /= cnt;
		for (int i = 0; i < H - 1; ++i)
		{
			updatech(i, sw, len);
		}
		for (int i = 0; i < W - 1; ++i)
		{
			updatecw(th, i, len);
		}
	}
}
void solve_last()
{
	auto [sh, sw, th, tw] = input();
	using T = std::tuple<int, int, int>;
	std::priority_queue<T, std::vector<T>, std::greater<>> pq;
	std::array<std::array<int, W>, H> d;
	std::array<std::array<int, W>, H> b;
	for (int i = 0; i < H; ++i)
	{
		for (int j = 0; j < W; ++j)
		{
			d[i][j] = inf;
			b[i][j] = -1;
		}
	}
	d[sh][sw] = 0;
	pq.emplace(0, sh, sw);
	const int dh[] = {1, 0, -1, 0};
	const int dw[] = {0, 1, 0, -1};
	const std::string dir = "DRUL";
	while(not pq.empty())
	{
		auto [dist, h, w] = pq.top();
		pq.pop();
		if(dist > d[h][w])
			continue;
		if(h == th and w == tw)
			break;
		for (int i = 0; i < 4; ++i)
		{
			const int nh = h + dh[i];
			const int nw = w + dw[i];
			if(nh < 0 or nh >= H or nw < 0 or nw >= W)
				continue;
			if(chmin(d[nh][nw], d[h][w] + cost(h, w, nh, nw)))
			{
				pq.emplace(d[nh][nw], nh, nw);
				b[nh][nw] = i;
			}
		}
	}
	int cnt = 0;
	std::string res;
	int curh = th, curw = tw;
	while(curh != sh or curw != sw)
	{
		const int idx = b[curh][curw];
		cnt += 1;
		res += dir[idx];
		curh -= dh[idx];
		curw -= dw[idx];
	}
	std::reverse(res.begin(), res.end());
	cout << res << endl;
	ll len; cin >> len;
	len /= cnt;
	curh = sh, curw = sw;
	for(const auto &c : res)
	{
		if(c == 'R')
		{
			updatecw(curh, curw, len);
			curw += 1;
		}
		if(c == 'L')
		{
			updatecw(curh, curw - 1, len);
			curw -= 1;
		}
		if(c == 'U')
		{
			updatech(curh - 1, curw, len);
			curh -= 1;
		}
		if(c == 'D')
		{
			updatech(curh, curw, len);
			curh += 1;
		}
	}
}

void solve(int kkt)
{
	if(kkt < 130)
	{
		solve_first();
	}
	else
	{
		solve_last();
	}
}
int main()
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  for (int i = 0; i < H; ++i)
  {
  	for (int j = 0; j < W; ++j)
  	{
  		chcount[i][j] = cwcount[i][j] = 1;
  		ch[i][j] = cw[i][j] = 4500;
  	}
  }
  int kkt = 1000;
  //cin >> kkt;
  while(kkt--)
    solve(1000 - kkt);
  return 0;
}