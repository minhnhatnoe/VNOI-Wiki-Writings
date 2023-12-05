# Suffix Automaton

Người viết: Nguyễn Minh Nhật - HUS High School for Gifted Students

## Lời mở đầu

Đã bao giờ bạn gặp các bài toán về xâu mà thuật toán trâu đơn giản chỉ là duyệt trâu $\frac{n * (n-1)}{2}$ xâu con `s[l, r]`?
- Tìm số lượng xâu con phân biệt?
- Xâu con thứ tự thứ $k$?

Đã bao giờ bạn sử dụng Suffix Array kèm với đủ thứ cấu trúc dữ liệu trời ơi đất hỡi?

Nếu câu trả lời là có, khả năng cao Suffix Automaton sẽ cho bạn một lời giải khác, đơn giản, tầm thường, dễ code và dễ nghĩ (nếu như bạn đã quen với cách nghĩ của Suffix Automaton).

## Giới thiệu

Suffix Automaton là một cấu trúc dữ liệu quản lý xâu rất mạnh, tương đương Suffix Tree, có khả năng thay thế toàn bộ chức năng của KMP, Z-function, Manacher, Suffix Array, Aho-Corasick,... và có thể được xây dựng online (thay đổi và truy vấn đan xen).

Độ phức tạp thời gian và không gian của Suffix Automaton là $O(n*c)$ với $c$ là độ lớn của bảng chữ cái. Trên thực tế, với một số tối ưu đơn giản sẽ được trình bày trong bài viết, thời gian chạy của Suffix Automaton sẽ tương đương với các thuật toán $O(n)$ nếu $c \approx 26$.

Nói một cách nôm na, Suffix Automaton là cách để bạn lưu thông tin về tất cả các xâu con của một xâu (hoặc một tập xâu) $S$.

## Kiến thức cần có

Trước khi đọc bài viết, bạn đọc nên hiểu rõ cấu trúc dữ liệu Aho-Corasick. Kiến thức về Suffix Array và mảng Longest Common Prefix (LCP) cũng sẽ hữu ích, nhưng không nhất thiết cần dùng để hiểu bài viết này. Bạn đọc cũng có thể sử dụng Suffix Automaton để hiểu rõ hơn Suffix Array.

## Định nghĩa


