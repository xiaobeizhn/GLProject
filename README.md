# Request
## 6.14
- [ ] 为Shader类添加构造函数，参数为已经编译完成的着色器的引用

- [ ] 编写ShaderManager类，采用单例模式
  - [ ] 能够在主函数开头加载所有Shader
  - [ ] 映射Shader名->Shader引用，封装成公共方法
  - [ ] 取出entry.cpp内在渲染循环内部的初始化，改为ShaderManager的get

- [ ] 分离mesh声明与定义

- [ ] 窗口视图调整问题

  