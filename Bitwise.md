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

## Toán tử BITSHIFT LEFT (<<)

Định nghĩa của toán tử Bitshift Left là dịch tất cả các bit trong một số nguyên sang trái một lượng nào đó. Nói cách khác, trong phép toán ```a << b```, các bit của ```a``` được dịch sang trái ```b``` lần.

Ví dụ, xét số ```5 = 0b101```, nếu thực hiện phép toán ```0b101<<2```, ta nhận được ```0b10100 = 20```.

Nếu quan sát kỹ, bạn sẽ nhận thấy một tính chất thú vị sau của phép toán Bitshift Left: ```a << b``` $= a * 2^b$. Ta có tính chất này do phép toán Bitshift Left có thể hiểu là thêm một số 0 

### Logical Shift, Arithmetic Shift

## Các toán tử thao tác Bit (Bitwise Operators)

Việc sử dụng các toán tử thao tác Bit có thể được hiểu nôm na là thực hiện các thao tác tương ứng trên từng Bit của các toán hạng (operands).

### Toán tử Bitwise AND (&)

Phép toán AND trả về True khi và chỉ khỉ cả hai toán hạng là True. Chẳng hạn, 

### Toán tử Bitwise OR

Phép toán OR có bảng chân 

### Toán tử Bitwise XOR

### Toán tử Bitwise NOT


## Các hàm thao tác Bit
[//]: <> (TODO: C++20 functions)
### Hàm POPCOUNT, PARITY

### Hàm CLZ, __lg

### Hàm CTZ, FFS

## Sử dụng toán tử Bit để tăng tốc cho code