### 拦截窗口：
- CreateWindow 创建窗口 
- CreateWindowEx(A) 创建窗口 
- ShowWindow 显示窗口 
- UpdateWindow 更新窗口 
- GetWindowText(A) 获取窗口文本 

### 拦截消息框： 
- MessageBox(A) 创建消息框 
- MessageBoxExA 创建消息框 
- MessageBoxIndirect(A) 创建定制消息框 
- IsDialogMessageW 

### 拦截警告声： 
- MessageBeep 发出系统警告声(如果没有声卡就直接驱动系统喇叭发声) 


### 拦截对话框： 
- DialogBox 创建模态对话框 
- DialogBoxParam(A) 创建模态对话框 
- DialogBoxIndirect 创建模态对话框 
- DialogBoxIndirectParam(A) 创建模态对话框 
- CreateDialog 创建非模态对话框 
- CreateDialogParam(A) 创建非模态对话框 
- CreateDialogIndirect 创建非模态对话框 
- CreateDialogIndirectParam(A) 创建非模态对话框 
- GetDlgItemText(A) 获取对话框文本 
- GetDlgItemInt 获取对话框整数值 


### 拦截剪贴板： 
- GetClipboardData 获取剪贴板数据 


### 拦截注册表： 
- RegOpenKey(A) 打开子健 
- RegOpenKeyEx 打开子健 
- RegQueryValue(A) 查找子健 
- RegQueryValueEx 查找子健 
- RegSetValue(A) 设置子健 
- RegSetValueEx(A) 设置子健 


### 功能限制拦截断点： 
- EnableMenuItem 禁止或允许菜单项 
- EnableWindow 禁止或允许窗口 


### 拦截时间： 
- GetLocalTime 获取本地时间 
- GetSystemTime 获取系统时间 
- GetFileTime 获取文件时间 
- GetTickCount 获得自系统成功启动以来所经历的毫秒数 
- GetCurrentTime 获取当前时间（16位） 
- SetTimer 创建定时器 
- TimerProc 定时器超时回调函数 
- GetDlgItemInt 得指定输入框整数值 
- GetDlgItemText 得指定输入框输入字符串 
- GetDlgItemTextA 得指定输入框输入字符串 


### 拦截文件： 
- CreateFileA 创建或打开文件 (32位) 
- OpenFile 打开文件 (32位) 
- ReadFile 读文件 (32位) 
- WriteFile 写文件 (32位) 
- GetModuleFileNameA 
- GetFileSize 
- Setfilepointer 
- fileopen 
- FindFirstFileA 
- ReadFile 

### 拦截驱动器： 
- GetDriveTypeA 获取磁盘驱动器类型 
- GetLogicalDrives 获取逻辑驱动器符号 
- GetLogicalDriveStringsA 获取当前所有逻辑驱动器的根驱动器路径 