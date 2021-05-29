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
int chboarder[W];
int cwboarder[H];
int chcount[H][W];
int cwcount[H][W];
int sh, sw, th, tw;
bool usedW[H], usedH[W];
std::vector<std::vector<int>> A;
std::vector<int> LIST[1800];
std::vector<int> B, C;
std::chrono::system_clock::time_point  start, end;

int enc(int h, int w, int dir)
{
	return dir * 900 + h * W + w;
}
std::tuple<int, int, int> dec(int X)
{
	return std::make_tuple(X / 900, (X % 900) / W,  X % W);
}
inline int cost(int X)
{
	auto [dir, h, w] = std::make_tuple(X / 900, (X % 900) / W,  X % W);
	if(dir == 0)
		return cw[h][w];
	else
		return ch[h][w];
}
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
			std::swap(sh, th);
		return ch[sh][sw];
	}
}

void updatech(int i, int j, int val)
{
	ll sum = 1LL * ch[i][j] * chcount[i][j] + val;
	chcount[i][j] += 1;
	ch[i][j] = sum / chcount[i][j];
}
void updatecw(int i, int j, int val)
{
	ll sum = 1LL * cw[i][j] * cwcount[i][j] + val;
	cwcount[i][j] += 1;
	cw[i][j] = sum / cwcount[i][j];
}
unsigned long xor128() {
  static unsigned long x=123456789, y=362436069, z=521288629, w=88675123;
  unsigned long t=(x^(x<<11));
  x=y; y=z; z=w;
  return ( w=(w^(w>>19))^(t^(t>>8)));
}
void solve_last()
{
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
	B.emplace_back(len);
	len /= cnt;
	curh = sh, curw = sw;
	std::vector<int> a;
	for(const auto &c : res)
	{
		if(c == 'R')
		{
			updatecw(curh, curw, len);
			a.emplace_back(enc(curh, curw, 0));
			curw += 1;
		}
		if(c == 'L')
		{
			updatecw(curh, curw - 1, len);
			a.emplace_back(enc(curh, curw - 1, 0));
			curw -= 1;
		}
		if(c == 'U')
		{
			updatech(curh - 1, curw, len);
			a.emplace_back(enc(curh - 1, curw, 1));
			curh -= 1;
		}
		if(c == 'D')
		{
			updatech(curh, curw, len);
			a.emplace_back(enc(curh, curw, 1));
			curh += 1;
		}
	}
	std::sort(a.begin(), a.end());
	A.emplace_back(a);
}

void solve_first()
{
	int curh = sh, curw = sw;
	int cnth = std::abs(sh - th);
	int cntw = std::abs(sw - tw);
	std::string res;
	std::vector<int> a;
	auto movew = [&](int tw)
	{
		while(curw < tw)
		{
			a.emplace_back(enc(curh, curw, 0));
			res += 'R';
			curw += 1;
		}
		while(curw > tw)
		{
			a.emplace_back(enc(curh, curw - 1, 0));
			res += 'L';
			curw -= 1;
		}
	};
	auto moveh = [&](int th)
	{
		while(curh < th)
		{
			a.emplace_back(enc(curh, curw, 1));
			res += 'D';
			curh += 1;
		}
		while(curh > th)
		{
			a.emplace_back(enc(curh - 1, curw, 1));
			res += 'U';
			curh -= 1;
		}
	};
	{
		if(cnth > cntw)
		{
			int w = -1;
			for (int i = std::min(sw, tw); i <= std::max(sw, tw); ++i)
			{
				if(not usedH[i])
				{
					w = i;
					// if(cnth > 15)
						usedH[w] = true;
					break;
				}
			}
			if(w == -1)
			{
				solve_last();
			}
			else
			{
				movew(w);
				moveh(th);
				movew(tw);
				cout << res << endl;
				ll len; cin >> len;
				B.emplace_back(len);
				len /= (cnth + cntw);
				if(usedH[w])
				{
					for (int i = 0; i < H - 1; ++i)
					{
						updatech(i, w, len);
					}
				}

				{
					int curh = sh, curw = sw;
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
				std::sort(a.begin(), a.end());
				A.emplace_back(a);
			}
		}
		else
		{
			int h = -1;
			for (int i = std::min(sh, th); i <= std::max(sh, th); ++i)
			{
				if(not usedW[i])
				{
					h = i;
					// if(cntw > 15)
						usedW[h] = true;
					break;
				}
			}
			if(h == -1)
			{
				solve_last();
			}
			else
			{
				moveh(h);
				movew(tw);
				moveh(th);
				cout << res << endl;
				ll len; cin >> len;
				B.emplace_back(len);
				len /= (cnth + cntw);
				if(usedW[h])
				{
					for (int i = 0; i < W - 1; ++i)
					{
						updatecw(h, i, len);
					}
				}
				{
					int curh = sh, curw = sw;
					for(const auto &c : res)
					{
						if(c == 'R')
						{
							// updatecw(curh, curw, len);
							curw += 1;
						}
						if(c == 'L')
						{
							// updatecw(curh, curw - 1, len);
							curw -= 1;
						}
						if(c == 'U')
						{
							// updatech(curh - 1, curw, len);
							curh -= 1;
						}
						if(c == 'D')
						{
							// updatech(curh, curw, len);
							curh += 1;
						}
					}
				}
				std::sort(a.begin(), a.end());
				A.emplace_back(a);
			}
		}
	}
}

void solve(int kkt)
{
	std::tie(sh, sw, th, tw) = input();
	if(kkt > 0)
	{
		auto st = std::chrono::system_clock::now();
		double deadline = pow(0.999998, 1000 - kkt) * kkt * 1.95;
		double start_temp = 10;
		double end_temp = 1;
		double now_time = std::chrono::duration_cast<std::chrono::milliseconds>(st-start).count();
		double TL = deadline - now_time;
		for (int jupi = 0;; ++jupi)
		{
			if(jupi % 10000 == 0)
			{
				end = std::chrono::system_clock::now();
				if(std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() > deadline)
					break;
				for (int i = 0; i < (int)A.size(); ++i)
				{
					C[i] = 0;
					for (int j = 0; j < (int)A[i].size(); ++j)
					{
						C[i] += cost(A[i][j]);
					}
				}
			}
			{
				const int row = xor128() % (int)A.size();
				const int col = xor128() % A[row].size();
				const int CHANGE = 30;
				int change = (int)(xor128() % CHANGE) - CHANGE / 2;
				const int cval = A[row][col];
				if(cost(cval) + change < 1000)
				{
					change = 1000 - cost(cval);
				}
				if(cost(cval) + change > 9000)
				{
					change = 9000 - cost(cval);
				}
				int diff = 0;
				for(const auto &idx : LIST[cval])
				{
					const int pc = C[idx];
					const int nc = C[idx] + change;
					int pd = std::abs(B[idx] - pc);
					int nd = std::abs(B[idx] - nc);
					diff += pd - nd;
				}
				double temp = start_temp + (end_temp - start_temp) 
				* std::chrono::duration_cast<std::chrono::milliseconds>(end-st).count() / TL;
				double prob = std::exp(diff / temp);
				if(prob >= (xor128() % (int)(1e6)) / 1e6)
				{
					auto [dir, h, w] = dec(cval);
					if(dir == 0)
					{
						cw[h][w] += change;
					}
					else
					{
						ch[h][w] += change;
					}
				}
			}
		}
	}
	if(kkt < 200)
	{
		solve_first();
	}
	else
	{
		solve_last();
	}
	if(kkt >= 300)
	{
		for (int i = 0; i < H; ++i)
		{
			double diff = 0.0;
			int id = -1;
			for (int j = 1; j < W; ++j)
			{
				double sl = 0.0;
				double sr = 0.0;
				for (int k = 0; k < W; ++k)
				{
					if(k < j)
						sl += cw[i][k];
					else
						sr += cw[i][k];
				}
				sl /= j;
				sr /= (W - j);
				if(chmax(diff, std::fabs(sl - sr)))
				{
					id = j;
				}
			}
			if(diff > 1000)
			{
				cwboarder[i] = id;
			}
		}
		for (int j = 0; j < W; ++j)
		{
			double diff = 0.0;
			int id = -1;
			for (int i = 1; i < H; ++i)
			{
				double su = 0.0;
				double sd = 0.0;
				for (int k = 0; k < H; ++k)
				{
					if(k < i)
						su += ch[k][j];
					else
						sd += ch[k][j];
				}
				su /= i;
				sd /= (H - i);
				if(chmax(diff, std::fabs(su - sd)))
				{
					id = i;
				}
			}
			if(diff > 1000)
			{
				chboarder[j] = id;
			}
		}
	}
	if(kkt >= 200)
	{
		for (int i = 0; i < H; ++i)
		{
			int sl = -1, sr = -1;
			for (int j = 0; j < W; ++j)
			{
				int sum = 0;
				int cnt = 0;
				if(cwboarder[i] == -1)
				{
					for (int k = 0; k < W; ++k)
					{
						if(std::abs(j - k) < W / 4)
						{
							sum += cw[i][k];
							cnt += 1;
						}
					}
					updatecw(i, j, sum / cnt);
				}
				else
				{
					if(sl == -1)
					{
						sl = 0, sr = 0;
						for (int k = 0; k < W; ++k)
						{
							if(k < cwboarder[i])
							{
								sl += cw[i][k];
							}
							else
							{
								sr += cw[i][k];
							}
						}
					}
					if(j < cwboarder[i])
					{
						updatecw(i, j, sl / cwboarder[i]);
					}
					else
					{
						updatecw(i, j, sr / (W - cwboarder[i]));
					}
				}
			}
		}
		for (int j = 0; j < W; ++j)
		{
			int su = -1, sd = -1;
			for (int i = 0; i < H; ++i)
			{
				if(chboarder[j] == -1)
				{
					int sum = 0;
					int cnt = 0;
					for (int k = 0; k < H; ++k)
					{
						if(std::abs(i - k) < H / 4)
						{
							sum += ch[k][j];
							cnt += 1;
						}
					}
					updatech(i, j, sum / cnt);
				}
				else
				{
					if(su == -1)
					{
						su = 0, sd = 0;
						for (int k = 0; k < H; ++k)
						{
							if(k < chboarder[j])
							{
								su += ch[i][k];
							}
							else
							{
								sd += ch[i][k];
							}
						}
					}
					if(i < chboarder[j])
					{
						updatech(i, j, su / chboarder[j]);
					}
					else
					{
						updatech(i, j, sd / (H - chboarder[j]));
					}
				}
			}
		}
	}
	for (const auto &e : A.back())
	{
		LIST[e].emplace_back(kkt);
	}
	C.resize(B.size());
}
int main()
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  start = std::chrono::system_clock::now();
  for (int i = 0; i < H; ++i)
  {
  	chboarder[i] = cwboarder[i] = -1;
  	for (int j = 0; j < W; ++j)
  	{
  		chcount[i][j] = cwcount[i][j] = 0;
  		ch[i][j] = cw[i][j] = 5000;
  	}
  }

  for (int kkt = 0; kkt < 1000; ++kkt)
  {
  	solve(kkt);
  }
  return 0;
}
