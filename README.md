# BattelShip
Cài đặt các thư viện cần thiết:
Trên hệ điều hành Linux, sử dụng lệnh:
sudo apt-get update  
sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev mysql-server  
Tải mã nguồn từ kho lưu trữ:
Truy cập kho lưu trữ trên GitHub và tải mã nguồn (hoặc clone bằng git clone).\n
Biên dịch chương trình:\n
Xóa thư mục build cũ (nếu có):\n
rm -rf build  \n
Tạo thư mục build mới:\n
mkdir build  \n
cd build  \n
Sử dụng cmake để tạo các file cấu hình:\n
cmake ..  \n
Biên dịch chương trình bằng lệnh make:\n
make \n
Chạy chương trình:\n
Khởi động server trước:\n
 ./Server  \n
Sau đó khởi động client:\n
 ./Client  \n
