#include <bits/stdc++.h>
using ll = long long;
using ull = unsigned long long;
using std::cin;
using std::cout;
using std::endl;
std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());
template<class T> inline bool chmax(T& a, T b) { if (a < b) { a = b; return 1; } return 0; }
template<class T> inline bool chmin(T& a, T b) { if (a > b) { a = b; return 1; } return 0; }
const int inf = (int)1e9 + 7;
const long long INF = 1LL << 60;

std::chrono::system_clock::time_point  start, end;
double start_temp, end_temp;
std::vector<int> pos[8];

void pos_init()
{
	for (int i = 0; i < 8; ++i)
	{
		pos[i].clear();
	}
}
uint32_t xor64(void) {
  static uint64_t x = 88172645463325252ULL;
  x = x ^ (x << 13); x = x ^ (x >> 7);
  return x = x ^ (x << 17);
}

template <class T> struct RollingHash {
	std::vector<ull> hash, pows;
	const ull base = 1206;
	RollingHash(const T &a)
		: hash(a.size() + 1), pows(a.size() + 1, 1) {
		for (int i = 0; i < (int)a.size(); i++) {
			pows[i + 1] = pows[i] * base;
			hash[i + 1] = hash[i] * base + a[i];
		}
	}
	// 現在の文字列のサイズ
	int size() { return hash.size() - 1; }
	// [l, r)
	ull get(int l, int r) {
		assert(l <= r);
		ull ret = hash[r] - hash[l] * pows[r - l];
		return ret;
	}
	void concat(const T &b) {
		int n = hash.size() - 1, m = b.size();
		pows.resize(n + m + 1);
		hash.resize(n + m + 1);
		for (int i = 0; i < m; i++) {
			pows[n + i + 1] = pows[n + i] * base;
			hash[n + i + 1] = hash[n + i] * base + b[i];
		}
	}
	void pop_back() {
		hash.pop_back();
		pows.pop_back();
	}
};

void solve()
{
	start_temp = 0.5;
	end_temp = 0.00001;
	std::mt19937 kkt(89);
	int _, m; cin >> _ >> m;
	const int n = 20;
	std::vector<std::string> vs(m);
	std::vector<RollingHash<std::string>> hash;
	hash.reserve(m);
	for (int i = 0; i < m; ++i)
	{
		cin >> vs[i];
		hash.emplace_back(RollingHash<std::string>(vs[i]));
	}
	std::vector<std::vector<int>> a;
	std::vector<int> r, gomi, nr, v, sv;
	
	std::vector<char> used(m);
	for (int i = 0; i < m; ++i)
	{
		r.emplace_back(i);
	}
	while (not r.empty() and (int)a.size() < n)
	{
		v.clear();
		std::shuffle(r.begin(), r.end(), kkt);
		int sl = (int)vs[r[0]].size();
		auto h = hash[r[0]];
		v.emplace_back(r[0]);
		used[r[0]] = true;
		for (int jupi = 1; jupi < (int)r.size(); jupi++)
		{
			const std::string &t = vs[r[jupi]];
			for (int i = 0; i < sl; ++i)
			{
				const int len = std::min(sl - i, (int)t.size());
				if(sl + (int)t.size() - len > n)
					continue;
				if(h.get(i, i + len) == hash[r[jupi]].get(0, len))
				{
					sl += (int)t.size() - len;
					h.concat(t.substr(len, (int)t.size() - len));
					v.emplace_back(r[jupi]);
					used[r[jupi]] = true;
					break;
				}
			}
		}
		for(const auto &e : r)
		{
			if(not used[e])
			{
				const std::string &t = vs[e];
				bool in = false;
				for (int i = 0; i <= sl - (int)t.size(); ++i)
				{
					if(h.get(i, i + (int)t.size()) == hash[e].get(0, (int)t.size()))
					{
						in = true;
						break;
					}
				}
				if(in)
				{
					v.emplace_back(e);
					used[e] = true;
				}
				else
				{
					gomi.emplace_back(e);
				}
			}
		}
		std::swap(gomi, r);
		gomi.clear();
		a.emplace_back(v);
	}
	const double deadline = 2950;
	for (int jupi_loves_kkt = 0;; jupi_loves_kkt++)
	{
		end = std::chrono::system_clock::now();
		const double time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
		if(time > deadline)
			break;
		int high = xor64() % n;
		nr = r;
		for(const auto &e : a[high])
		{
			nr.emplace_back(e);
		}
		std::shuffle(nr.begin(), nr.end(), kkt);
		v.clear();
		pos_init();
		int sl = (int)vs[nr[0]].size();
		for (int i = 0; i < (int)vs[nr[0]].size(); ++i)
		{
			pos[vs[nr[0]][i] - 'A'].emplace_back(i);
		}
		auto h = hash[nr[0]];
		v.emplace_back(nr[0]);
		for (int jupi = 1; jupi < (int)nr.size(); jupi++)
		{
			if(sl == n)
				break;
			const std::string &t = vs[nr[jupi]];
			for (const auto &i : pos[t[0] - 'A'])
			{
				const int len = std::min(sl - i, (int)t.size());
				if(sl + (int)t.size() - len > n)
					break;
				if(h.get(i, i + len) == hash[nr[jupi]].get(0, len))
				{
					for (int j = 0; j < (int)t.size() - len; j++)
					{
						pos[t[len + j] - 'A'].emplace_back(sl + j);
					}
					sl += (int)t.size() - len;
					h.concat(t.substr(len, (int)t.size() - len));
					v.emplace_back(nr[jupi]);
					break;
				}
			}
		}
		sv = v;
		std::sort(sv.begin(), sv.end());
		gomi.clear();
		for(const auto &e : nr)
		{
			if(not std::binary_search(sv.begin(), sv.end(), e))
			{
				const std::string &t = vs[e];
				bool in = false;
				for (const auto &i : pos[t[0] - 'A'])
				{
					if(i + (int)t.size() > sl)
						break;
					if(h.get(i, i + (int)t.size()) == hash[e].get(0, (int)t.size()))
					{
						in = true;
						break;
					}
				}
				if(in)
				{
					gomi.emplace_back(e);
				}
			}
		}
		for(const auto &e : gomi)
			v.emplace_back(e);
		gomi.clear();
		const int score = (int)v.size() - (int)a[high].size();
		const double temp = start_temp + (end_temp - start_temp) * time / deadline;
		const double prob = std::exp((double)score / temp);

		if(prob > (xor64() % inf) / (double)inf)
		{
			for(const auto &e : a[high])
			{
				used[e] = false;
			}
			for(const auto &e : v)
			{
				used[e] = true;
			}
			r.clear();
			for(const auto &e :nr)
			{
				if(not used[e])
					r.emplace_back(e);
			}
			a[high] = v;
		}
	}
	std::vector<std::vector<char>> res(n, std::vector<char>(n, '.'));
	for (int h = 0; h < (int)a.size(); ++h)
	{
		const auto &v = a[h];
		std::string s = vs[v[0]];
		for (int jupi = 1; jupi < (int)v.size(); jupi++)
		{
			const std::string &t = vs[v[jupi]];
			for (int i = 0; i < (int)s.size(); i++)
			{
				const int len = std::min((int)s.size() - i, (int)t.size());
				if(s.substr(i, len) == t.substr(0, len))
				{
					s += t.substr(len, (int)t.size() - len);
					break;
				}
			}
		}
		for (int i = 0; i < (int)s.size(); ++i)
		{
			res[h][i] = s[i];
		}
	}
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << res[i][j];
		}
		cout << "\n";
	}
}

int main()
{
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);

  start = std::chrono::system_clock::now();
  int kkt = 1; 
  //cin >> kkt;
  while(kkt--)
    solve();
  return 0;
}