# Phép toán bit

Người viết: Nguyễn Minh Nhật - HUS High School for Gifted Students

## Giới thiệu

Các phép toán với bit (Bitwise Operators) là một tập hợp các toán tử và hàm dành riêng cho việc thực hiện các thao tác biến đổi và tính toán trên các bit của một số nguyên.

## Lưu ý trước khi đọc bài viết

Một số đoạn code trong bài viết chỉ đảm bảo hoạt động với compiler GCC. Các đoạn này sẽ được viết kèm theo mục Chú ý ở phía dưới.

Các khái niệm sau được sử dụng xuyên suốt bài viết:
- **Bảng chân lý (Truth Table)** của một toán tử Bit có thể hiểu nôm na là tất cả các trường hợp đầu vào/đầu ra của phép toán đó. Ví dụ, phép toán AND trong C++ giữa hai giá trị bool có thể được biểu diễn như sau

<center>

|a|b|AND|
|---|---|---|
|1|1|1|
|1|0|0|
|0|1|0|
|0|0|0|

</center>

- **Biểu diễn dạng nhị phân của một số** được đánh dấu bằng tiền tố ```0b```. Chẳng hạn, với số ```12``` có biểu diễn nhị phân là ```1100```, ta viết ```12 = 0b1100```. Đây cũng là cách viết được chấp nhận trong code C++.

- **Bitmask** là một chuỗi các chữ số $0$, $1$, hay các bit. Trong đó, việc bit thứ $i$ được bật tương ứng với việc một phần tử/đại lượng thứ $i$ nào đó được sử dụng.

    Ví dụ đơn giản nhất của bitmask là biểu diễn một tập con của một tập hợp $A$ cho trước. Chẳng hạn, $A = \{5, 1, 2, 3, 0, 4\}$, bitmask ```0b110010``` biểu diễn cho tập con $\{1, 0, 4\}$ của $A$.

    Chú ý rằng, trong bài viết này, thứ tự các bit của bitmask được đánh số từ phải sang trái, bắt đầu từ $0$. Điều này tương tự như chữ số hàng đơn vị, hàng chục, hàng trăm, hàng nghìn trong số thập phân lần lượt được viết từ trái sang phải, từ thấp đến cao.

    Trên thực tế, ta sẽ biểu diễn bitmask bằng các số nguyên (ví dụ như các kiểu ```int``` hay ```long long``` trong C++), hoặc các cấu trúc dữ liệu bit như ```bitset``` của C++.

- Trong một bitmask, **bit thứ $i$ bật** có nghĩa là bit thứ $i$ của bitmask này có giá trị bằng $1$. Tương tự, ***bit thứ $i$ tắt$ có nghĩa là bit thứ $i$ của bitmask này có giá trị bằng $0$.

## Các toán tử thao tác Bit (Bitwise Operators) cơ bản

### Toán tử BITSHIFT LEFT (<<)

Định nghĩa của toán tử Bitshift Left là dịch tất cả các bit trong một số nguyên sang trái một lượng nào đó. Nói cách khác, trong phép toán ```a << b```, các bit của ```a``` được dịch sang trái ```b``` lần.

Ví dụ, xét số ```5 = 0b101```, nếu thực hiện phép toán ```0b101<<2```, ta nhận được ```0b10100 = 20```.

Nếu quan sát kỹ, bạn sẽ nhận thấy một tính chất thú vị sau của phép toán Bitshift Left: ```a << b``` $= a * 2^b$. Ta có tính chất này do phép toán Bitshift Left ```a<<b``` có thể hiểu là thêm ```b``` chữ số $0$ vào cuối biểu diễn nhị phân của số ```a```. Điều này tương tự như việc thêm một chữ số $0$ vào cuối biểu diễn thập phân của một số sẽ nhân số đó thêm 10 lần.

#### Chú ý với C++

Trong trường hợp phép toán của bạn bị tràn số (bit $1$ được left shift đến quá giới hạn của kiểu số đang sử dụng), các bit bị tràn sẽ được coi như là $0$, và biến mất.

[//]: <> (TODO: Determine whether overflow is UB)

Trong phép toán ```a << b```, nếu giá trị của ```b``` lớn hơn hoặc bằng số lượng bit mà kiểu số của kết quả hỗ trợ ($64$ đối với ```(unsigned) long long``` và $32$ đối với ```(unsigned) int```), kết quả trả về của phép toán là không xác định.

### Toán tử BITSHIFT RIGHT (>>)

Nếu như Left Shift là thêm chữ số $0$ vào bên phải của một số nguyên ở dạng nhị phân, ta có thể hiểu Right Shift là xóa các chữ số ở bên phải. 

#### Phân biệt Logical Right Shift và Arithmetic Right Shift

Việc sử dụng các toán tử thao tác Bit có thể được hiểu nôm na là thực hiện các thao tác tương ứng trên từng Bit của các toán hạng (operands).

### Toán tử Bitwise AND (&)

Phép toán AND trả về True khi và chỉ khỉ cả hai toán hạng là True. Chẳng hạn, 

### Toán tử Bitwise OR

Phép toán OR có bảng chân 

### Toán tử Bitwise XOR

### Toán tử Bitwise NOT

### Ứng dụng

#### Sửa và truy cập bit

Một ứng dụng thường thấy của các phép toán Bitshift là đọc và sửa từng bit trong một Bitmask.

Chẳng hạn, để truy cập bit thứ $i$ trong bitmask $A$, ta có thể sử dụng phép toán ```(A >> i) % 2```. Trước khi đọc giải thích của phép toán này, hãy dành ra một chút thời gian để tự mình chạy thử một số ví dụ.

Xét ```A = 0b1010010```. Để truy cập bit thứ $4$, ta thực hiện phép toán ```(0b1010010 >> 4) % 2 = 0b101 % 2 = 1```. Xét phần đầu tiên của phép toán, ```A >> i```, ta nhận thấy rằng, về bản chất, phần này thực hiện thao tác đưa bit thứ $i$ về vị trí $0$. Để truy cập bit thứ $0$ này, ta lấy số dư khi chia cho $2$ của số này.

Một số cách truy cập khác của 

## Các hàm thao tác Bit
[//]: <> (TODO: C++20 functions)
### Hàm POPCOUNT, PARITY

### Hàm CLZ, __lg

### Hàm CTZ, FFS

## Sử dụng toán tử Bit để tăng tốc cho code