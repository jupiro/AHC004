#include <bits/stdc++.h>
using ll = long long;
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
void solve()
{
	start_temp = 0.1;
	end_temp = 0.001;
	std::mt19937 kkt(89);
	int _, m; cin >> _ >> m;
	const int n = 20;
	std::vector<std::string> vs(m);
	for (int i = 0; i < m; ++i)
	{
		cin >> vs[i];
	}
	std::vector<std::pair<std::string, std::vector<int>>> a;
	std::vector<int> r, gomi, used(m), nr;
	for (int i = 0; i < m; ++i)
	{
		r.emplace_back(i);
	}
	while (not r.empty() and (int)a.size() < n)
	{
		std::string s;
		std::vector<int> v;
		std::shuffle(r.begin(), r.end(), kkt);
		s = vs[r[0]];
		v.emplace_back(r[0]);
		used[r[0]] = true;
		for (int jupi = 1; jupi < (int)r.size(); jupi++)
		{
			const std::string &t = vs[r[jupi]];
			for (int i = 0; i < (int)s.size(); ++i)
			{
				const int len = std::min((int)s.size() - i, (int)t.size());
				if((int)s.size() + (int)t.size() - len > n)
					continue;
				if(s.substr(i, len) == t.substr(0, len))
				{
					s += t.substr(len, (int)t.size() - len);
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
				for (int i = 0; i <= (int)s.size() - (int)t.size(); ++i)
				{
					if(s.substr(i, (int)t.size()) == t)
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
		a.emplace_back(std::make_pair(s, v));
	}
	const double deadline = 2950;
	while(true)
	{
		end = std::chrono::system_clock::now();
		const double time = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
		if(time > deadline)
			break;
		int h = rnd() % n;
		nr = r;
		for(const auto &e : a[h].second)
		{
			nr.emplace_back(e);
		}
		std::shuffle(nr.begin(), nr.end(), kkt);
		std::string s;
		std::vector<int> v;
		s = vs[nr[0]];
		v.emplace_back(nr[0]);
		for (int jupi = 1; jupi < (int)r.size(); jupi++)
		{
			const std::string &t = vs[nr[jupi]];
			for (int i = 0; i < (int)s.size(); ++i)
			{
				const int len = std::min((int)s.size() - i, (int)t.size());
				if((int)s.size() + (int)t.size() - len > n)
					continue;
				if(s.substr(i, len) == t.substr(0, len))
				{
					s += t.substr(len, (int)t.size() - len);
					v.emplace_back(nr[jupi]);
					break;
				}
			}
		}
		std::sort(v.begin(), v.end());
		gomi.clear();
		for(const auto &e : nr)
		{
			if(not std::binary_search(v.begin(), v.end(), e))
			{
				const std::string &t = vs[e];
				bool in = false;
				for (int i = 0; i <= (int)s.size() - (int)t.size(); ++i)
				{
					if(s.substr(i, (int)t.size()) == t)
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
		const int score = (int)v.size() - a[h].second.size();
		const double temp = start_temp + (end_temp - start_temp) * time / deadline;
		const double prob = exp(score / temp);

		if(prob > (kkt() % inf) / (double)inf)
		{
			for(const auto &e : a[h].second)
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
			a[h] = std::make_pair(s, v);
		}
	}
	std::vector<std::vector<char>> res(n, std::vector<char>(n, '.'));
	for (int i = 0; i < (int)a.size(); ++i)
	{
		for (int j = 0; j < (int)a[i].first.size(); ++j)
		{
			res[i][j] = a[i].first[j];
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