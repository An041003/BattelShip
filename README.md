# BattelShip
Cài đặt các thư viện cần thiết:<br>
Trên hệ điều hành Linux, sử dụng lệnh:<br>
sudo apt-get update  <br>
sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev mysql-server  <br>
Tải mã nguồn từ kho lưu trữ:<br>
Truy cập kho lưu trữ trên GitHub và tải mã nguồn (hoặc clone bằng git clone).<br>
Biên dịch chương trình:<br>
Xóa thư mục build cũ (nếu có):<br>
rm -rf build  <br>
Tạo thư mục build mới:<br>
mkdir build  <br>
cd build  <br>
Sử dụng cmake để tạo các file cấu hình:<br>
cmake ..  <br>
Biên dịch chương trình bằng lệnh make:<br>
make <br>
Chạy chương trình:<br>
Khởi động server trước:<br>
 ./Server  <br>
Sau đó khởi động client:<br>
 ./Client  <br>
