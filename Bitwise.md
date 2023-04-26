# Phép toán bit

Người viết: Nguyễn Minh Nhật - HUS High School for Gifted Students

## Giới thiệu

Các phép toán với bit (Bitwise Operators) là một tập hợp các toán tử và hàm dành riêng cho việc thực hiện các thao tác biến đổi và tính toán trên các bit của một số nguyên.

## Lưu ý trước khi đọc bài viết

Một số đoạn code trong bài viết chỉ đảm bảo hoạt động với compiler GCC. Các đoạn này sẽ được viết kèm theo mục Chú ý ở phía dưới.

Các khái niệm sau được sử dụng xuyên suốt bài viết:

- **Bảng chân lý (Truth Table)** của một toán tử Bit có thể hiểu nôm na là tất cả các trường hợp đầu vào/đầu ra của phép toán đó. Sau đây là bảng chân lý của một số các toán tử sẽ được giới thiệu trong bài viết này

<center>

|a|b|AND|OR|XOR|
|:-|:-|:-|:-|:-|
|1|1|1|1|0|
|1|0|0|1|1|
|0|1|0|1|1|
|0|0|0|0|0|

</center>

- **Biểu diễn dạng nhị phân của một số** được đánh dấu bằng tiền tố ```0b```. Chẳng hạn, với số ```12``` có biểu diễn nhị phân là ```1100```, ta viết ```12 = 0b1100```. Đây cũng là cách viết được chấp nhận trong code C++.

- **Bitmask** là một chuỗi các chữ số $0$, $1$, hay các bit. Trong đó, việc bit thứ $i$ được bật tương ứng với việc một phần tử/đại lượng thứ $i$ nào đó được sử dụng.

    Ví dụ đơn giản nhất của bitmask là biểu diễn một tập con của một tập hợp $A$ cho trước. Chẳng hạn, $A = \{5, 1, 2, 3, 0, 4\}$, bitmask ```0b110010``` biểu diễn cho tập con $\{1, 0, 4\}$ của $A$.

    Chú ý rằng, trong bài viết này, thứ tự các bit của bitmask được đánh số từ phải sang trái, bắt đầu từ $0$. Điều này tương tự như chữ số hàng đơn vị, hàng chục, hàng trăm, hàng nghìn trong số thập phân lần lượt được viết từ trái sang phải, từ thấp đến cao.

    Trên thực tế, ta sẽ biểu diễn bitmask bằng các số nguyên (ví dụ như các kiểu ```int``` hay ```long long``` trong C++), hoặc các cấu trúc dữ liệu bit như ```bitset``` của C++.

- Trong một bitmask, **bit thứ $i$ bật** có nghĩa là bit thứ $i$ của bitmask này có giá trị bằng $1$. Tương tự, **bit thứ $i$ tắt** có nghĩa là bit thứ $i$ của bitmask này có giá trị bằng $0$.

## Các toán tử thao tác Bit (Bitwise Operators) cơ bản

### Toán tử BITSHIFT LEFT (<<)

Định nghĩa của toán tử Bitshift Left là dịch tất cả các bit trong một số nguyên sang trái một lượng nào đó. Nói cách khác, trong phép toán ```a << b```, các bit của ```a``` được dịch sang trái ```b``` lần.

Ví dụ, xét số ```5 = 0b101```, nếu thực hiện phép toán ```0b101<<2```, ta nhận được ```0b10100 = 20```.

Nếu quan sát kỹ, bạn sẽ nhận thấy một tính chất thú vị sau của phép toán Bitshift Left: ```a << b``` $= a * 2^b$. Ta có tính chất này do phép toán Bitshift Left ```a<<b``` có thể hiểu là thêm ```b``` chữ số $0$ vào cuối biểu diễn nhị phân của số ```a```. Điều này tương tự như việc thêm một chữ số $0$ vào cuối biểu diễn thập phân của một số sẽ nhân số đó thêm 10 lần.

#### Chú ý với C++

Trong trường hợp phép toán của bạn bị tràn số (bit $1$ được left shift đến quá giới hạn của kiểu số đang sử dụng), sẽ có 2 trương hợp xảy ra:
1. Nếu kiểu số của kết quả là một số ```unsigned```, các bit bị tràn sẽ được coi như là $0$, và biến mất.
2. Nếu kiểu số của kết quả là một số ```signed```, chương trình của bạn sẽ bị UB. Tuy nhiên, trong hầu hết trường hợp, code của bạn sẽ không bị lỗi, mà chỉ trả về một kết quả không xác định nào đó.

Trong phép toán ```a << b```, nếu giá trị của ```b``` là âm hoặc lớn hơn hoặc bằng số lượng bit mà kiểu số của kết quả hỗ trợ ($64$ đối với ```(unsigned) long long``` và $32$ đối với ```(unsigned) int```), kết quả trả về của phép toán là không xác định.

### Toán tử BITSHIFT RIGHT (>>)

Nếu như Left Shift là thêm chữ số $0$ vào bên phải của một số nguyên ở dạng nhị phân, ta có thể hiểu Right Shift là xóa các chữ số ở bên phải.

Ví dụ, xét số ```13 = 0b1101```, ta có ```0b1101 >> 2 = 0b11```.

Tương tự với Bitshift Left, ta cũng có tính chất ```a >> b``` $= \lfloor \frac{a}{2^b} \rfloor$ với $a$ nguyên không âm

#### Phân biệt Logical Right Shift và Arithmetic Right Shift

Riêng đối với Right Shift, hầu hết các cấu trúc máy tính cung cấp hai loại phép toán khác nhau.

Khác biệt duy nhất giữa Logical Right Shift và Arithmetic Right Shift là Logical Right Shift điền các bit bên trái mới được thêm đều là $0$, trong khi Arithmetic Right Shift điền các bit này là giá trị của bit trái cùng trong số ban đầu (bit thứ $31$ đối với kiểu ```int```, và bit thứ $63$ đối với kiểu ```long long```).

Chẳng hạn, ta sử dụng kiểu số ```char``` có 8 bit, và thực hiện phép toán ```0b```**```101```**```01101 >> 5```. Logical Right Shift sẽ trả về kết quả ```0b00000```**```101```**, nhưng Arithmetic Right Shift sẽ trả về ```0b11111```**```101```**.

Chắc chắn khi đọc đến đây, các bạn sẽ tự hỏi về ý nghĩa của phép Arithmetic Right Shift. Trong trường hợp toán hạng ```a``` là số không âm, hai phép toán hoạt động tương đương. Tuy nhiên, trong trường hợp ```a``` âm, phép Logical Right Shift không có ý nghĩa về mặt toán học, mà đơn giản chỉ là đẩy các bit sang phải. Trong khi đó, phép Arithmetic Right Shift sẽ vẫn đảm bảo tính chất ```a >> b``` $= \lfloor \frac{a}{2^b} \rfloor$. Chú ý rằng kết quả của phép toán sẽ được làm tròn xuống, chẳng hạn như ```-7 >> 2``` $= \frac{-7}{2^2} = -1.75$ được làm tròn xuống $-2$.

Lý do phép toán trên hoạt động là vì các số nguyên âm được biểu diễn bằng dạng two's complement. Do giới hạn của bài viết, người viết sẽ không đi sâu hơn vào loại biểu diễn này.

Trong C++, phép Logical Right Shift sẽ được sử dụng nếu toán tử đầu tiên là một số thuộc loại ```unsigned```, còn nếu không thì máy sẽ sử dụng Arithmetic Right Shift.

### Toán tử Bitwise AND (&), OR (|) và XOR (^)

Việc sử dụng ba toán tử này có thể được hiểu nôm na là thực hiện các thao tác tương ứng trên từng Bit của các toán hạng (operands). Nói cách khác, nếu ký hiệu $a_i$ là bit thứ $i$ của bitmask $a$, việc thực hiện phép toán $c = a \oplus b$ trong đó $a, b, c$ là các bitmask và $\oplus$ là một phép toán nào đó sẽ tương đương với việc thực hiện $c_i = a_i \oplus b_i \forall 0 \leq i$.

Định nghĩa của các phép toán này như sau:

1. **AND** trả về True khi và chỉ khi cả hai toán hạng là True.

    Ví dụ, ta có ```0b11100010 & 0b10101111 = 0b10100010```
2. **OR** trả về True khi và chỉ khi ít nhất một toán hạng là True.

    Ví dụ, ta có ```0b11100010 & 0b10101111 = 0b11101111```.
3. **XOR** trả về True khi và chỉ khi hai toán hạng có giá trị khác nhau. Một cách hiểu khác cho **XOR** là phép cộng theo modulo 2.

    Ví dụ, ta có ```0b11100010 & 0b10101111 = 0b01001101```.


### Toán tử Bitwise NOT (~)

Toán tử Bitwise NOT có lẽ là toán tử đơn giản nhất. Toán tử này nhận vào một toán hạng $A$ trả về phần bù của toán hạng này. Nói cách khác, định nghĩa của NOT là trả về False khi và chỉ khi toán hạng là True.

Ví dụ, ta có ```~0b10100100 = 0b01011011``` (trong trường hợp đầu vào là kiểu số có 8 bit).

Cần chú ý, do máy tính chỉ quan tâm tới số lượng bit của kiểu số đầu vào, những bit không sử dụng ở bên trái cũng sẽ được bật lên. Chẳng hạn, khi thực hiện phép ```0b10``` với kiểu số ```char``` (có 8 bit), ta nhận được ```0b11111101``` thay vì ```0b01```. Trong đa số trường hợp, ta sẽ cần phải tắt các bit được bật thừa này đi.

## Các hàm thao tác Bit

C++ hiện nay hỗ trợ một số các hàm liên quan tới xử lý bit giúp ta thực hiện một số các phép tính thông dụng với độ phức tạp thời gian $O(1)$.

### Hàm POPCOUNT

Từ chuẩn C++20 trở lên, thư viện chuẩn của C++ cung cấp hàm ```std::popcount(int x)```. Hàm này trả về số lượng bit bật trong bitmask $x$.

Chẳng hạn, ta có ```std::popcount(0b100101) = 3```.

Đối với các chuẩn C++ cũ hơn, compiler GCC cung cấp các hàm tương tự là ```std::__builtin_popcount(x)``` cho kiểu ```unsigned int``` và ```std::__builtin_popcountll(x)``` cho kiểu ```unsigned long long```.

Chú ý: Đối với các hàm có dạng ```std::__builtin```, thêm đuôi ```ll``` sẽ gọi hàm đó với kiểu đầu vào là ```unsigned long long```.

Ngoài ra, GCC cũng cung cấp hàm ```std::__builtin_parity(x)``` trả về ```std::popcount(x) % 2```. Hàm này thường được sử dụng trong các bài toán liên quan tới bao hàm loại trừ.

### Hàm COUNTL_ZERO

Từ chuẩn C++20 trở lên, thư viện chuẩn của C++ cung cấp hàm ```std::countl_zero(x)``` trả về số lượng bit $0$ ở bên trái của biến đầu vào.

Chẳng hạn, ```std::countl_zero(int(0b10)) == 30``` (do kiểu ```int``` có 32 bit).

GCC cũng có hàm ```std::__builtin_clz(x)``` (count leading zeroes). Tuy nhiên, hàm này trả về kết quả không xác định đối với ```x == 0```.

Phép toán ```31 - std::__builtin_clz(x)``` hay ```63 - std::__builtin_clzll(x)``` trả về $\lfloor \log_2(x) \rfloor$, thường được sử dụng trong cài đặt của Bảng thưa (Sparse Table) hoặc FFT.

### Hàm COUNTR_ZERO

Từ chuẩn C++20 trở lên, thư viện chuẩn của C++ cung cấp hàm ```std::countr_zero(x)``` trả về số lượng bit $0$ ở bên phải của biến đầu vào.

Hàm tương đương của GCC là ```std:::__builtin_ctz(x)``` (count trailing zeroes). Tuy nhiên hàm này có giá trị không xác định với ```x == 0```. GCC cũng cung cấp một hàm khác là ```std::__builtin_ffs(x) == std::__builtin_ctz(x) + 1```. Trong trường hợp ```x == 0```, hàm này trả về $0$.

### Ứng dụng

#### Truy cập Bit

Một ứng dụng thường thấy của các phép toán Bit là đọc và sửa từng bit trong một bitmask.

Chẳng hạn, để truy cập bit thứ $i$ trong bitmask $A$, ta có thể sử dụng phép toán ```A & (1<<i)```. Trước khi đọc giải thích của phép toán này, hãy dành ra một chút thời gian để tự mình chạy thử một số ví dụ.

Xét ```A = 0b1010010```. Để truy cập bit thứ $4$, ta thực hiện phép toán ```0b1010010 & (1<<4) = 0b1010010 & 0b10000 = 0b10000```. Xét phần thứ hai của phép toán, ```1<<i```, ta nhận thấy rằng, về bản chất, phần này thực hiện thao tác tạo ra một bitmask chỉ có bit thứ $i$ bật. Bitmask này khi được AND với bitmask ban đầu sẽ loại bỏ thông tin của tất cả mọi bit ngoại trừ bit thứ $i$.

Ngoài ra cũng có một số các cách khác để truy cập bit, ví dụ như ```(A >> i) % 2```, hay ```(A >> i) & 1```.

#### Chỉnh sửa Bit

Sử dụng phương pháp tương tự như phần trên, ta có một số phép sửa Bit như sau:

1. Gán một Bit bằng $0$ với ```A & ~(1<<i)```.
2. Gán một Bit bằng $1$ với ```A | (1<<i)```.
3. Flip một Bit (từ $0$ sang $1$ hoặc từ $1$ sang $0$) với ```A ^ (1<<i)```.

#### Tắt các Bit cao nhất của một bitmask

Để lấy các bit trong khoảng từ $0$ tới $i-1$ của một bitmask, hay đồng loạt tắt tất cả các bit từ $i$ trở đi, ta có thể sử dụng ```A & ((1<<i)-1)```. Phép toán ```((1<<i) - 1)``` tạo ra bitmask mà trong đó chỉ các bit từ $0$ tới $i-1$ được bật lên.

#### Biểu diễn tập hợp

Như đã nói ở phần đầu bài viết, ứng dụng đơn giản nhất của bitmask là biểu diễn một tập con của một tập $A$ cho trước nào đó. Từ ứng dụng này, ta có một dạng bài tên là quy hoạch động trạng thái (dp bitmask).

Khi đó, các phép toán AND, OR, XOR, NOT lần lượt tương ứng với các phép lấy giao, lấy hợp, lấy hiệu đối xứng, và lấy phần bù của tập hợp.

Các phép toán tập hợp khác cũng có thể được biểu diễn bằng bitmask, ví dụ như:

1. Kiểm tra $A$ là tập con của $B$ bằng ```A & B == A```.
2. Tạo tập hợp $A$ chỉ có phần tử thứ $i$ bằng ```1 << i```.
3. Hiệu của hai tập hợp $A$ và $B$ bằng ```(A ^ B) & A```.
4. Phần bù của tập hợp $B$ trong $A$ băng ```A & ~B```.

#### Lặp qua mọi tập con

Để lặp qua mọi tập con của $S$, ta viết vòng lặp ```for``` như sau:

```c++
for (int i=S; true; i = (i-1) & S) {
    // Thực hiện thao tác nào đó với tập con i của S
    if (i == 0) break;
}
```

Độ phức tạp của vòng lặp trên là $2^{|S|}$, chính là số tập con của $S$. Như vậy, nếu như ta lặp mọi tập $S$ từ $0$ tới $2^n$, sau đó lặp mọi tập con của $S$, độ phức tạp thời gian sẽ là $3^n$.

#### Cài đặt cấu trúc dữ liệu Fenwick Tree

Cách cài đặt [Fenwick Tree](https://vnoi.info/wiki/algo/data-structures/fenwick.md) tối ưu cũng là một trong những ứng dụng thú vị của các toán tử Bit.

#### Giải các bài toán bao hàm loại trừ

#### Tăng tốc cho code
