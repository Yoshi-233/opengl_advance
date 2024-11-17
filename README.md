# opengl_senior

### 1. Gamma
### 2. Normal_Mapping
法线贴图id = 2
### 3. TBN计算
![](.README_images/ed35ddb1.png)
![](.README_images/61fadd44.png)
![](.README_images/79f460ac.png)
![](.README_images/dedb2382.png)
求得法线后，注意此时的法线是模型的法线不是采样的法线，当计算完TBN矩阵后， 将这个矩阵
应用到法线采样器上。
![](.README_images/c8f008ef.png)

### 5.视差贴图
![](.README_images/435e882a.png)
![](.README_images/1ab64d04.png)
![](.README_images/bcc22987.png)
![](.README_images/bfe981fb.png)
![](.README_images/6e82d0f3.png)