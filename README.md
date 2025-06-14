# Embedded_System
Hệ thống phát hiện và cảnh báo rò rỉ khí gas trong nhà

CPU: vi điều khiển sử dụng vi xử lý STM32 thực hiện đọc cảm biến, xử lý ngưỡng và điều khiển các thiết bị cảnh báo.

Các đầu vào:
- Hệ thống sử dụng cảm biến gas như MQ-2 để phát hiện nồng độ khí gas (LPG, methane).
- SW1 để chuyển đổi hệ thống giữa trạng thái hoạt động và trạng thái dừng. Khi hệ thống dừng hoạt động, nếu bấm SW1 bộ đếm chuyển sang trạng thái hoạt động, và ngược lại. Khi chuyển từ trạng thái dừng sang trạng thái hoạt động, hệ thống tiếp tục giám sát trạng thái của cảm biến đo khí gas và hiển thị trên LCD.
- SW2 để xóa (reset) trạng thái hệ thống về trạng thái ban đầu.
Các đầu ra:
- LED xanh dương khi không có khí gas
- LED vàng khi nồng độ khí gas thấp
- LED đỏ nhấp nháy với tần số 1Hz khi nồng độ khí gas cao
- LED đỏ nháy ở tần số 2-10Hz khi nồng độ khí cao trên mức nguy hiểm – tần số nháy càng cao khi nồng độ khí gas càng cao
- LED xanh lá cây khi hệ thống ở trạng thái dừng hoạt động.
- Màn hình LCD: Hiển thị nồng độ gas, trạng thái hệ thống:
   + Hiện thị trạng thái của hệ thống: không hoạt động / hoạt động
   + Trạng thái cảnh báo: an toàn / nồng độ khí gas thấp / nồng độ khí gas cao / nồng độ khí gas cao trên mức nguy hiểm
- Buzzer: còi hú khi nồng độ khí gas vượt mức nguy hiểm
- Rơ-le: điều khiển quạt lọc không khí khi nồng độ khí gas cao.
- Sử dụng timer để xác định thời gian nhấp nháy các LED.
- Gửi cảnh báo qua Internet (sử dụng ESP32 gửi cảnh báo về email).
