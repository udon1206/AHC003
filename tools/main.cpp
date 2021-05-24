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

using D = double;
const D EPS = 1e-10;

// matrix
template<class T> struct Matrix {
    std::vector<std::vector<T> > val;
    Matrix(int n, int m, T x = 0) : val(n, std::vector<T>(m, x)) {}
    void init(int n, int m, T x = 0) {val.assign(n, std::vector<T>(m, x));}
    size_t size() const {return val.size();}
    inline std::vector<T>& operator [] (int i) {return val[i];}
};

template<class T> int GaussJordan(Matrix<T> &A, bool is_extended = false) {
    int m = A.size(), n = A[0].size();
    int rank = 0;
    for (int col = 0; col < n; ++col) {
        // 拡大係数行列の場合は最後の列は掃き出ししない
        if (is_extended && col == n-1) break;

        // ピボットを探す
        int pivot = -1;
        T ma = EPS;
        for (int row = rank; row < m; ++row) {
            if (abs(A[row][col]) > ma) {
                ma = abs(A[row][col]);
                pivot = row;
            }
        }
        // ピボットがなかったら次の列へ
        if (pivot == -1) continue;

        // まずは行を swap
        swap(A[pivot], A[rank]);

        // ピボットの値を 1 にする
        auto fac = A[rank][col];
        if(fac == 0)
        	assert(false);
        for (int col2 = 0; col2 < n; ++col2) A[rank][col2] /= fac;

        // ピボットのある列の値がすべて 0 になるように掃き出す
        for (int row = 0; row < m; ++row) {
            if (row != rank && abs(A[row][col]) > EPS) {
                auto fac = A[row][col];
                for (int col2 = 0; col2 < n; ++col2) {
                    A[row][col2] -= A[rank][col2] * fac;
                }
            }
        }
        ++rank;
    }
    return rank;
}

template<class T> std::vector<T> linear_equation(Matrix<T> A, std::vector<T> b) {
    // extended
    int m = A.size(), n = A[0].size();
    Matrix<T> M(m, n + 1);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) M[i][j] = A[i][j];
        M[i][n] = b[i];
    }
    int rank = GaussJordan(M, true);
    
    // check if it has no solution
    std::vector<T> res;
    // for (int row = rank; row < m; ++row) if (abs(M[row][n]) > EPS) return res;

    // answer
    res.assign(n, 5000);
    for (int i = 0; i < rank; ++i) res[i] = M[i][n];
    return res;
}
std::tuple<int, int, int, int> input()
{
	int si, sj, ti, tj;
	cin >> si >> sj >> ti >> tj;
	return std::make_tuple(si, sj, ti, tj);
}
const int H = 30;
const int W = 30;
// int ch[H][W];
// int cw[H][W];
// int chcount[H][W];
// int cwcount[H][W];
int sh, sw, th, tw;
// bool usedW[H], usedH[W];
std::vector<std::vector<double>> A;
std::vector<double> B;
std::vector<double> x;
inline int cost(int sh, int sw, int th, [[maybe_unused]] int tw)
{
	if(sh == th)
	{
		return (int)std::max(std::round(x[sh]), 0.01);
		// if(sw > tw)
		// 	std::swap(sw, tw);
		// return cw[sh][sw];
	}
	else
	{
		return (int)std::max(std::round(x[H + sw]), 0.01);
		// if(sh > th)
		// 	std::swap(sh, sw);
		// return ch[sh][sw];
	}
}

// void updatech(int i, int j, int val)
// {
// 	ll sum = 1LL * ch[i][j] * chcount[i][j] + val;
// 	chcount[i][j] += 1;
// 	ch[i][j] = sum / chcount[i][j];
// }
// void updatecw(int i, int j, int val)
// {
// 	ll sum = 1LL * cw[i][j] * cwcount[i][j] + val;
// 	cwcount[i][j] += 1;
// 	cw[i][j] = sum / cwcount[i][j];
// }
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
	std::vector<bool> used(H * W);
	while(not pq.empty())
	{
		auto [dist, h, w] = pq.top();
		pq.pop();
		if(used[h * W + w])
			continue;
		used[h * W + w] = true;
		if(h == th and w == tw)
			break;
		for (int i = 0; i < 4; ++i)
		{
			const int nh = h + dh[i];
			const int nw = w + dw[i];
			if(nh < 0 or nh >= H or nw < 0 or nw >= W)
				continue;
			if(used[nh * W + nw])
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
	curh = sh, curw = sw;
	std::vector<double> v(H + W);
	for(const auto &c : res)
	{
		if(c == 'R')
		{
			v[curh] += 1;
			curw += 1;
		}
		if(c == 'L')
		{
			v[curh] += 1;
			curw -= 1;
		}
		if(c == 'U')
		{
			v[H + curw] += 1;
			curh -= 1;
		}
		if(c == 'D')
		{
			v[H + curw] += 1;
			curh += 1;
		}
	}
	A.emplace_back(v);
}

// void solve_first()
// {
// 	int curh = sh, curw = sw;
// 	int cnth = std::abs(sh - th);
// 	int cntw = std::abs(sw - tw);
// 	std::string res;
// 	auto movew = [&](int tw)
// 	{
// 		while(curw < tw)
// 		{
// 			res += 'R';
// 			curw += 1;
// 		}
// 		while(curw > tw)
// 		{
// 			res += 'L';
// 			curw -= 1;
// 		}
// 	};
// 	auto moveh = [&](int th)
// 	{
// 		while(curh < th)
// 		{
// 			res += 'D';
// 			curh += 1;
// 		}
// 		while(curh > th)
// 		{
// 			res += 'U';
// 			curh -= 1;
// 		}
// 	};
// 	if(std::max(cnth, cntw) * 10 < (cnth + cntw) * 7)
// 	{
// 		movew(tw);
// 		moveh(th);
// 		cout << res << endl;
// 		ll len; cin >> len;
// 		len /= (cnth + cntw);
// 		for (int i = std::min(sw, tw); i <= std::max(sw, tw); ++i)
// 		{
// 			updatecw(sh, i, len);
// 		}
// 		for (int i = std::min(sh, th); i <= std::max(sh, th); ++i)
// 		{
// 			updatech(i, tw, len);
// 		}
// 	}
// 	else
// 	{
// 		if(cnth > cntw)
// 		{
// 			int w = -1;
// 			for (int i = std::min(sw, tw); i <= std::max(sw, tw); ++i)
// 			{
// 				if(not usedH[i])
// 				{
// 					w = i;
// 					if(cnth > 10)
// 						usedH[w] = true;
// 					break;
// 				}
// 			}
// 			if(w == -1)
// 			{
// 				solve_last();
// 			}
// 			else
// 			{
// 				movew(w);
// 				moveh(th);
// 				movew(tw);
// 				cout << res << endl;
// 				ll len; cin >> len;
// 				len /= (cnth + cntw);
// 				for (int i = 0; i < H - 1; ++i)
// 				{
// 					updatech(i, w, len);
// 				}
// 			}
// 		}
// 		else
// 		{
// 			int h = -1;
// 			for (int i = std::min(sh, th); i <= std::max(sh, th); ++i)
// 			{
// 				if(not usedW[i])
// 				{
// 					h = i;
// 					if(cntw > 10)
// 						usedW[h] = true;
// 					break;
// 				}
// 			}
// 			if(h == -1)
// 			{
// 				solve_last();
// 			}
// 			else
// 			{
// 				moveh(h);
// 				movew(tw);
// 				moveh(th);
// 				cout << res << endl;
// 				ll len; cin >> len;
// 				len /= (cnth + cntw);
// 				for (int i = 0; i < W - 1; ++i)
// 				{
// 					updatecw(h, i, len);
// 				}
// 			}
// 		}
// 	}
// }

void solve(int kkt)
{

	std::tie(sh, sw, th, tw) = input();
	solve_last();
	{
		Matrix<double> mat(A.size(), A[0].size());
		for (int i = 0; i < (int)A.size(); ++i)
		{
			for (int j = 0; j < (int)A[i].size(); ++j)
			{
				mat[i][j] = A[i][j];
			}
		}
		x = linear_equation(mat, B);
	}
}
int main()
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  x.resize(H + W, 5000);
  int kkt = 1000;
  //cin >> kkt;
  while(kkt--)
    solve(1000 - kkt);
  return 0;
}