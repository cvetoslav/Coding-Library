#include <bits/stdc++.h>
#define endl '\n'

//#pragma GCC optimize ("O3")
//#pragma GCC target ("sse4")

using namespace std;
template<class T, class T2> inline int chkmax(T &x, const T2 &y) { return x < y ? x = y, 1 : 0; }
template<class T, class T2> inline int chkmin(T &x, const T2 &y) { return x > y ? x = y, 1 : 0; }
const int MAXN = (1 << 20);

random_device rd;
mt19937_64 mt(rd());

struct node
{
	int sz, prior, id;
	node *par, *pp, *l, *r;
	node() { id = 0; sz = 0; prior = 0; par = nullptr; l = nullptr; r = nullptr; }
	node(int v) { id = v; sz = 1; prior = mt(); l = nullptr; r = nullptr; }
};

typedef node* pnode;

int size(pnode v) { return v ? v->sz : 0; }

void pull(pnode &v) 
{ 
	if(!v) return;
	
	v->par = nullptr;
	v->sz = size(v->l) + size(v->r) + 1; 
	
	if(v->l) v->l->par = v;
	if(v->r) v->r->par = v;

	if(v->l && v->l->pp) v->pp = v->l->pp, v->l->pp = nullptr;
	if(v->r && v->r->pp) v->pp = v->r->pp, v->r->pp = nullptr;
}

void merge(pnode &t, pnode l, pnode r)
{
	if(!l) { t = r; return; }
	if(!r) { t = l; return; }

	if(l->prior > r->prior)
		merge(l->r, l->r, r), t = l;
	else
		merge(r->l, l, r->l), t = r;

	pull(t);
}

void split(pnode t, pnode &l, pnode &r, int k, int add = 0)
{
	if(!t) { l = nullptr; r = nullptr; return; }

	int idx = add + size(t->l);
	if(idx <= k)
		split(t->r, t->r, r, k, idx + 1), l = t;
	else
		split(t->l, l, t->l, k, add), r = t;

	pull(t);
}

pnode get_root(pnode t) { if(!t) return nullptr; while(t->par) t = t->par; return t; }

pnode remove_right(pnode t)
{
	pnode rt = t;

	int pos = size(rt->l);
	while(rt->par)
	{
		if(rt->par->r == rt) pos += size(rt->par->l) + 1;
		rt = rt->par;
	}

	pnode l, r, pp = rt->pp;
	rt->pp = nullptr;
	split(rt, l, r, pos);

	l->pp = pp;
	if(r) r->pp = t;
	
	return l;
}

pnode remove_left(pnode t)
{
	pnode rt = t;

	int pos = size(rt->l);
	while(rt->par)
	{
		if(rt->par->r == rt) pos += size(rt->par->l) + 1;
		rt = rt->par;
	}

	pnode l, r, pp = rt->pp;
	rt->pp = nullptr;
	split(rt, l, r, pos - 1);

	l->pp = pp;
	return r;
}

pnode merge_trees(pnode u, pnode t)
{
	u = get_root(u);
	t = get_root(t);
	t->pp = nullptr;
	merge(u, u, t);
	return u;
}

struct link_cut_tree
{
	pnode ver[MAXN];

	pnode access(pnode t)
	{
		t = remove_right(t);
		while(t->pp)
		{
			pnode u = t->pp;
			u = remove_right(u);
			t = merge_trees(u, t);
		}

		return t;
	}

	pnode find_root(pnode u)
	{
		u = access(u);
		while(u->r) u = u->r;
		access(u);
		return u;
	}

	void link(pnode u, pnode w)
	{
		access(u);
		access(w);
		merge_trees(w, u);
	}

	void cut(pnode p)
	{
		access(p);
		remove_left(p);
	}

	int depth(pnode u)
	{
		u = access(u);
		return size(u);
	}

	pnode lca(pnode u, pnode v)
	{
		if(u == v) return u;
		if(depth(u) > depth(v)) swap(u, v);

		access(v);
		access(u);

		return get_root(v)->pp;
	}

	/// normal functions
	void init(int c) { for(int i = 0; i <= c; i++) ver[i] = new node(i); }
	inline int lca(int u, int v) { return lca(ver[u], ver[v])->id; }
	inline int root(int u) { return find_root(ver[u])->id; }
	inline void link(int u, int v) { link(ver[u], ver[v]); }
	inline int depth(int u) { return depth(ver[u]); }
	inline void cut(int u) { cut(ver[u]); }
};

int n, m;

void read()
{
	cin >> n >> m;
}

link_cut_tree lct;

void solve()
{
	lct.init(n);

	while(m--)
	{
		string type;
		cin >> type;

		if(type == "link")
		{
			int u, w;
			cin >> u >> w;
			lct.link(u, w);
		}
		else if(type == "lca")
		{
			int u, v;
			cin >> u >> v;
			cout << lct.lca(u, v) << endl;
		}
		else if(type == "cut")
		{
			int u;
			cin >> u;
			lct.cut(u);
		}

	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	read();
	solve();
	return 0;
}

