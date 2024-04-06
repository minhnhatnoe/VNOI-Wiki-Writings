# Aho Corasick

Người viết: Nguyễn Minh Nhật - HUS High School for Gifted Students

Reviewer:

## Giới thiệu

Trước khi đọc bài viết này, bạn cần trang bị kiến thức về các chủ đề sau:

- [Trie (Cây tiền tố)](https://vnoi.info/wiki/algo/data-structures/trie.md) - Cấu trúc dữ liệu giúp quản lý một tập hợp các xâu.
- [Thuật toán Knuth-Morris-Pratt (KMP)](https://vnoi.info/wiki/algo/string/kmp.md) - Thuật toán giúp tìm các lần xuất hiện của một xâu trong xâu khác. Dù cũng không quá cần thiết, kiến thức về KMP Automaton sẽ giúp bạn hiểu rõ bài này hơn.

Nếu như đã học qua Trie và thuật toán KMP, đã bao giờ bạn nghĩ tới việc kết hợp chúng chưa? Điều gì sẽ xảy ra nếu ta có thể chạy KMP trên nhiều xâu cùng một lúc?

Aho-Corasick là một thuật toán giúp bạn quản lý một tập xâu và trả lời bài toán:

**Cho $N$ xâu $S_i$ và $Q$ xâu $T_i$. Với mỗi xâu $T_i$, liệt kê tất cả các lần xuất hiện của các xâu $S_i$ ở trong xâu $T_i$ này.**

Độ phức tạp của Aho-Corasick là:

- Xây dựng trong $O(c*\sum{|S_i|})$ với $c$ là số lượng chữ cái khác nhau trong $N$ xâu $S_i$ và $|S_i|$ là số chữ cái trong xâu $S_i$. Giả sử bài toán cho các xâu chữ cái la-tinh viết thường, độ phức tạp là $26 * \sum{|S_i|}$.
- Truy vấn trong $O(|T|)$, với $|T|$ là độ dài xâu truy vấn.

## Ký hiệu

Trong bài viết này, ta quy ước các ký hiệu như sau:

- Nếu không có giải thích gì thêm, $c$ là độ lớn của bảng chữ cái (thường là $26$ - số chữ cái từ `A-Z`).
- Với $S$ là một xâu, $|S|$ là độ dài của xâu $S$.
- Với $S$ là một xâu, $S_i$ $(0 \leq i < |S|)$ là chữ cái thứ $i$ trong xâu $S$. Ta quy ước $S_i \in [0, c-1)$
- Với $S$ là một xâu, $S_{l..r}$ $(0 \leq l \leq r < |S|)$ là xâu con liên tiếp từ $l$ tới $r$ của xâu S.

Ngoài ra, trong bài viết này, ta tạm gọi cấu trúc dữ liệu được xây dựng bởi thuật toán Aho-Corasick là cây Aho-Corasick.

## Xây dựng Trie

Cách xây dựng Trie, các bạn có thể tham khảo ở bài viết về [Trie (Cây tiền tố)](https://vnoi.info/wiki/algo/data-structures/trie.md). Để xây dựng cây Aho-Corasick, ta xây dựng Trie đối với tập xâu $S$. Cài đặt của bước này như sau:

```c++=
struct trie{
    struct node{
        int cnt, nxt[26];
        node(): cnt(0) {fill(nxt, nxt+26, -1);}
    };
    vector<node> g = {node()};
    void insert_string(const string &s){
        int ptr = 0;
        for (char c: s){
            if (g[ptr].nxt[c - 'a'] == -1){
                g[ptr].nxt[c - 'a'] = g.size();
                g.emplace_back();
            }
            ptr = g[ptr].nxt[c - 'a'];
        }
        g[ptr].cnt++;
    }
};
```

Ở mỗi nút của Trie, ta lưu một biến `cnt` là số lượng xâu trong tập $S$ kết thúc ở nút này. Tùy vào câu hỏi cụ thể của bài toán, biến này có thể lưu các giá trị khác.

**Chú ý:** theo tính chất của cấu trúc dữ liệu Trie, mỗi nút trong cây sẽ đại diện cho một **tiền tố của một xâu $S_i$** nào đó. Tính chất này sẽ cần sử dụng trong các phần sau.

## Phân tích thuật toán Knuth-Morris-Pratt

Trước khi đi vào thuật toán Aho-Corasick, hãy nhìn lại cách hoạt động của thuật toán [Knuth-Morris-Pratt](https://vnoi.info/wiki/algo/string/kmp.md).

```c++
vector<int> prefix_function(const string &s){
    vector<int> pi(s.size());
    for (size_t i=1, p=0; i<s.size(); i++){
        for (; p && s[p] != s[i]; p = pi[p-1]);
        p = pi[i] = p + (s[p] == s[i]);
    }
    return pi;
}
void print_matches(const string &s, const string &t){
    vector<int> pi = prefix_function(s);
    for (size_t i=0, p=0; i<t.size(); i++){
        for (; p && s[p] != t[i]; p = pi[p-1]);
        p = p + (s[p] == t[i]);
        if (p == s.size())
            cout << i << "\n";
    }
}
```

Hàm `print_matches(s, t)` ở trên in ra tất cả các lần xuất hiện của xâu $S$ trong xâu $T$. Hàm này thực hiện

- Xây dựng hàm tiền tố `pi` cho xâu $S$.
- Khởi tạo con trỏ `p` trên xâu $S$, ban đầu trỏ về vị trí $0$.
- Duyệt lần lượt qua từng chữ cái của xâu $T$. Con trỏ `p` nhảy theo hàm `pi` cho tới khi tìm được vị trí mà $S_p = T_i$. Sau mỗi bước duyệt, con trỏ chỉ tới **tiền tố dài nhất của $S$** mà trùng với một **hậu tố của $T_{0..i}$**.

## Xây dựng cây Aho-Corasick

Hãy quay lại với cài đặt Trie ở phía trên. Ta sẽ xây dựng một cấu trúc tương đương "hàm tiền tố" của KMP cho Trie.

Cụ thể, với mỗi nút $P$ không phải nút gốc, ta xây dựng "liên kết hậu tố (suffix link)" trỏ tới một nút $V$ có tính chất: xâu được biểu diễn bởi $V$ là **hậu tố dài nhất** (khác $P$) của $P$.

### Giải bài toán ban đầu

Trước khi đi vào cách xây dựng liên kết hậu tố, ta sẽ giải một bài toán tương đương với bài toán ban đầu. Bài toán như sau:

Cho tập $N$ xâu $S$ và xâu $T$. Liệt kê tất cả các lần xuất hiện của các xâu thuộc $S$ trong xâu $T$.

Giả sử đã xây dựng được liên kết hậu tố cho tất cả các nút trong Trie. Tương tự với Knuth-Morris-Pratt, ta khởi tạo một con trỏ `p` chỉ tới gốc của Trie và duyệt từng chữ cái trong xâu $T$.

Sau mỗi bước duyệt qua từng chữ cái của xâu $T$, ta duy trì tính chất: Con trỏ `p` chỉ tới nút Trie sâu nhất (hay biểu diễn cho xâu dài nhất) mà trùng với một **hậu tố của $T_{0..i}$**.

Do việc trỏ tới một nút trong Trie chính là trỏ tới **tiền tố của một (vài) xâu $S_i$ nào đó**, ta có thể hình dung việc này như thực hiện Knuth-Morris-Pratt đồng thời trên tất cả các xâu trong tập $S$.

Để duy trì được tính chất này, con trỏ `p` sẽ nhảy theo liên kết hậu tố tới khi tìm được vị trí `p` mà **có cạnh đi tiếp với nhãn $T_i$**. Phép kiểm tra này tương đương với việc kiểm tra $S_p = T_i$ của Knuth-Morris-Pratt trên tất cả các xâu cùng lúc (và dừng lại nếu có một xâu thoả mãn).

Để hiểu rõ hơn về các bước chạy của thuật toán này, chúng ta sẽ chạy thuật toán với `T = "diduduadi"` và `S = {"di", "du", "didu", "dudua", "duadi", "didi"}`.

<!-- Thêm ví dụ -->

**Chú ý:** Để tìm được tất cả các xâu trùng với một hậu tố của $T_{0..i}$ tương ứng với con trỏ `p`, ta cần phải tìm trên tất cả các nút tới được bằng cách nhảy một (vài) bước từ `p` theo liên kết hậu tố.

### Xây dựng Automaton

Nhìn lại quy trình giải bài toán trên, ta nhận thấy Việc 

### Xây dựng liên kết hậu tố và cài đặt

Sau đây là cài đặt Aho-Corasick của người viết, xây dựng liên kết hậu tố từ Trie đã xây dựng từ bước trên.

```c++
struct aho_corasick{
    struct node{
        int slink, cnt, nxt[26], go[26];
        node(): slink(-1), cnt(0) {fill(nxt, nxt+26, -1);}
    };
    vector<node> g = {node()};
    void insert_string(const string &s){
        int ptr = 0;
        for (char c: s){
            if (g[ptr].nxt[c - 'a'] == -1){
                g[ptr].nxt[c - 'a'] = g.size();
                g.emplace_back();
            }
            ptr = g[ptr].nxt[c - 'a'];
        }
        g[ptr].cnt++;
    }
    void build_automaton(){
        for (deque<int> q = {0}; q.size(); q.pop_front()){
            int v = q.front();
            for (int i=0; i<26; i++){
                int &nxt = g[v].nxt[i];
                if (nxt != -1){
                    g[v].go[i] = nxt;
                    g[nxt].slink = (v == 0 ? 0 : g[g[v].slink].go[i]);
                    q.push_back(nxt);
                }
                else g[v].go[i] = (v == 0 ? 0 : g[g[v].slink].go[i]);
            }
        }
    }
};
```

Tương tự như hàm tiền tố của Knuth-Morris-Pratt, liên kết hậu tố `slink` của một nút $P$ sẽ biểu diễn cho **hậu tố dài nhất** (khác $P$) của $P$. Rõ ràng, `slink` của một nút sẽ là một nút có độ sâu nhỏ hơn (hay độ dài của xâu được biểu diễn ngắn hơn). Như vậy, để xây dựng cây, ta có thể sử dụng thuật toán duyệt theo chiều sâu BFS.

Đối với nút gốc, ta đặt `slink := -1` do liên kết hậu tố của nút gốc không được định nghĩa.

Đối với nút là con trực tiếp của nút gốc trên Trie, rõ ràng xâu được biểu diễn bởi nút chỉ có $1$ chữ cái và `slink` của nút phải trỏ về gốc.

Nhớ rằng, khi thực hiện việc duyệt xâu $T$ trên một cây Aho-Corasick, ta nhảy `p` (`p = g[p].slink`) tới khi có thể đi được trên mảng `nxt` (`g[p].nxt[t[i]] != -1`). Với mảng `go`, ta lưu nút `g[p].nxt[i]` này. Quan sát ví dụ, bạn đọc hãy thử tự chứng minh công thức `g[nxt].slink = g[g[v].slink].go[i]` (gợi ý: `g[v].slink` là hậu tố dài nhất của `v`, và ta đang thêm chữ cái `i` vào cả hai xâu).

Rõ ràng, `g[v].go[i] = g[v].nxt[i]` nếu `g[v].nxt[i] != -1`. Đối với trường hợp, `g[v].nxt[i] == -1`, việc thực hiện `g[v].go[i] = g[g[v].slink].go[i]` tương đương với việc ta thực hiện một lần nhảy `p`, sau đó cố gắng đi theo cạnh `i`. Trong trường hợp ở nút liên kết hậu tố `g[v].slink` cũng không đi được, `g[g[v].slink].go[i]` sẽ dẫn tới các nút cha, cho tới khi nào ta có thể đi được theo cạnh `i` (hoặc không thể đi được và phải đi về nút gốc).

## Xây dựng liên kết thoát

Với mỗi tiền tố $X = T_{0..i}$ tương ứng với con trỏ $p$, nếu nhảy theo liên kết hậu tố, ta có thể tìm thấy được tất cả xâu $Y$ thuộc $S$ trùng với hậu tố của xâu $X$. Nói cách khác, với mỗi $0 \leq i <|T|$, ta tìm được một $j$ sao cho $T_{j..i}$ thuộc $S$. Tuy nhiên, việc tìm kiếm này là $O(n)$.

Với giới hạn đầu vào $10^5$, ta có thể tìm được tất cả các $j$ hay không? Quan trọng hơn, số lượng $j$ thoả mãn điều kiện này là bao nhiêu? Sau đây, ta sẽ chứng minh số lượng vị trí $j$ thoả mãn với mỗi vị trí $i$ là $O(\sqrt{\sum{|S_k|}})$ ($\sum{|S_k|}$ là tổng số chữ cái trong các xâu thuộc $S$).

Gọi $Y$ là tập các xâu thuộc $S$ mà tồn tại $j$ sao cho $Y = T_{j..i}$. Do các vị trí $j$ khác nhau, độ dài các xâu thuộc $Y$ khác nhau. Rõ ràng, $\sum{|Y_i|} \geq \frac{|Y| * |Y+1|}{2}$ (xâu ngắn nhất trong $Y$ có độ dài $\geq 1$, xâu ngắn thứ hai có độ dài $\geq 2$, ...). Như vậy, $|Y| = O(\sqrt{\sum{|S_i|}})$. Chứng minh hoàn tất.

Nói một cách nôm na, với mỗi tiền tố $X$ của $T$ (có $|T|$ $X$ như vậy), có $O(\sqrt{\sum{|S_i|}})$ hậu tố của $X$ trùng với một xâu thuộc $S$. Chú ý rằng, tất cả các hậu tố của các tiền tố chính là tất cả các xâu con liên tiếp. Như vậy, có $O(|T| * \sqrt{\sum{|S_i|}})$ lần xuất hiện của các xâu thuộc $S$ trong xâu $T$.

Do số lượng lần nhảy theo liên kết hậu tố từ một vị trí là $O(n)$, để có thể tìm kiếm nhanh chóng tất cả các hậu tố như vậy, ta lưu thêm "liên kết thoát" (exit link) trên mỗi nút. Liên kết thoát của `p` sẽ trỏ tới nút `q` sao cho khi nhảy theo liên kết hậu tố từ `p`, `q` sẽ là nút đầu tiên mà `cnt != 0` (đồng nghĩa với việc tồn tại xâu được biểu diễn bởi nút `q` trong $S$).

## Cài đặt hoàn chỉnh

Sau đây là cài đặt hoàn chỉnh của người viết. Tùy vào mục đích sử dụng, bạn có thể cần thêm các biến trên mỗi node hoặc chỉnh sửa số lượng chữ cái. Trong cài đặt này, người viết sử dụng chung mảng `nxt` và `go` để giúp tiết kiệm bộ nhớ và đơn giản hoá cài đặt.

```c++
struct aho_corasick{
    struct node{
        int slink, elink, cnt, nxt[26];
        node(): slink(-1), elink(-1), cnt(0) {fill(nxt, nxt+26, -1);}
    };
    vector<node> g = {node()};
    void insert_string(const string &s){
        int ptr = 0;
        for (char c: s){
            if (g[ptr].nxt[c - 'a'] == -1){
                g[ptr].nxt[c - 'a'] = g.size();
                g.emplace_back();
            }
            ptr = g[ptr].nxt[c - 'a'];
        }
        g[ptr].cnt++;
    }
    void build_automaton(){
        for (deque<int> q = {0}; q.size(); q.pop_front()){
            int v = q.front();
            for (int i=0; i<26; i++){
                int &nxt = g[v].nxt[i];
                if (nxt == -1)
                    nxt = (v == 0 ? 0 : g[g[v].slink].nxt[i]);
                else{
                    g[nxt].slink = (v == 0 ? 0 : g[g[v].slink].nxt[i]);
                    g[nxt].elink = (g[v].cnt ? v : g[v].elink);
                    q.push_back(nxt);
                }
            }
        }
    }
};
```

## 

## Xử lý query thay đổi tập xâu

Dễ thấy rằng thuật toán xây dựng của chúng ta là một thuật toán offline (tập xâu $S$ không được thay đổi). Trên thực tế, có một số bài toán đòi hỏi thêm bớt các xâu qua từng truy vấn. Đối với những bài toán này, ta có các cách giải quyết như sau:

### Xử lý Offline

Nếu bài toán cho các truy vấn từ đầu, ta có thể nhập tất cả các truy vấn, sau đó xây dựng cây Aho-Corasick trên các truy vấn này. Ta xây dựng cây thứ hai với các cạnh là các liên kết hậu tố, sau đó quản lý các nút qua từng truy vấn bằng cách sử dụng Cây phân đoạn và [Đường đi Euler trên cây](https://vnoi.info/wiki/algo/graph-theory/euler-tour-on-tree.md). Do giới hạn của bài viết này, người viết sẽ không đi sâu hơn.

### Xử lý Online

Trong một số trường hợp, bài toán yêu cầu tạo ra xâu sử dụng kết quả từ truy vấn trước (ta không thể nhập tất cả các truy vấn sau trước khi đưa ra kết quả cho truy vấn trước). Trong trường hợp này, người viết biết tới hai cách xử lý.

#### Tăng độ phức tạp thêm $O(\lg Q)$

Người viết xin phép chỉ trình bày cách xử lý truy vấn thêm xâu; truy vấn xoá xâu là bài tập dành cho bạn đọc.

Ta lưu $\lg Q$ cây Aho-Corasick khác nhau; cây thứ $i$ có độ lớn là $2^i$. Với truy vấn $1$, ta thêm vào cây thứ $0$ rồi xây dựng luôn. Với truy vấn $2$, ta lấy xâu ở truy vấn $1$ và truy vấn $2$, thêm vào cây thứ $1$ rồi xây dựng, và xoá cây thứ $0$. Với truy vấn $3$, ta lại thêm vào cây thứ $0$ rồi xây dựng luôn.

Cứ như vậy, với truy vấn thứ $i$, ta cố gắng thêm vào cây $1$. Nếu cây $1$ đã đầy, ta lấy thêm hết xâu ở cây $1$ rồi thêm vào cây $2$. Nếu cây $2$ đã đầy, ta lấy thêm hết và thêm vào cây $3$, cứ như vậy. Nếu nháp các truy vấn này, bạn đọc có thể dễ dàng chứng minh được ở cây $i$ sẽ có hoặc $0$ hoặc $2^i$ xâu.

Độ phức tạp được cộng thêm là $O(\lg Q)$, do sau mỗi thao tác, một xâu đang ở tập $i$ chỉ có thể được đưa về các tập sau tập $i$. Ta chỉ có $\lg Q$ tập như vậy vì tập $\lg Q$ sẽ lưu được $\geq Q$ xâu. Như vậy, mỗi xâu chỉ có thể bị xây dựng lại tối đa $\lg Q$ lần.

Khi có truy vấn hỏi, ta thực hiện các truy vấn này với từng cây Aho-Corasick. Như vậy, độ phức tạp của tất cả các thao tác bị nhân thêm $O(\lg Q)$.

#### Giữ nguyên độ phức tạp

Cách xây dựng giữ nguyên độ phức tạp được mô tả trong paper [Incremental string matching](https://se.inf.ethz.ch/~meyer/publications/string/string_matching.pdf). Tuy nhiên, trên thực tế, các bài toán thuộc dạng này là rất hiếm. Trong trường hợp gặp, thay vì sử dụng kỹ thuật trên, bạn đọc có thể sử dụng các kỹ thuật mạnh hơn như Suffix Automaton.
