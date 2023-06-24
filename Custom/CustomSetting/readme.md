

<h1 align = "center">CustomSetting</h1>



![image-20230624214316666](./assets/image-20230624214316666.png)

# 简介

程序设置选项对话框

---

# 窗口结构

![image-20230624221656112](./assets/image-20230624221656112.png)

左边使用`QListWidget`控制右边的`QStackedWidget`

---

# 代码结构

1. 设置窗口分装成为一个单例
2. 使用`QSetting`保存信息

