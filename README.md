# BattelShip
Cài đặt các thư viện cần thiết:
Trên hệ điều hành Linux, sử dụng lệnh:
sudo apt-get update  
sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev mysql-server  
Tải mã nguồn từ kho lưu trữ:
Truy cập kho lưu trữ trên GitHub và tải mã nguồn (hoặc clone bằng git clone).
Biên dịch chương trình:
Xóa thư mục build cũ (nếu có):
rm -rf build  
Tạo thư mục build mới:
mkdir build  
cd build  
Sử dụng cmake để tạo các file cấu hình:
cmake ..  
Biên dịch chương trình bằng lệnh make:
make 
Chạy chương trình:
Khởi động server trước:
 ./Server  
Sau đó khởi động client:
 ./Client  
