# Basic Skill

## 請解釋下編譯指令，每一個參數代表的意涵 (10%)

```bash
g++-8 -std=c++17 -O2 -Wall -Wextra -fPIC -I./ -shared AITemplate/Porting.cpp -o ./build/a1.so
```
```
以下是我們的解釋:
#-O2:
-O means optimizing.It can improve the performance and/or code size at the expense of compilation time and may have the ability of debugging the program. And –O2 means optimizing more. As compared to -O, this option increases both compilation time and the performance of the generated code.簡而言之就是對code size跟execution time做最佳化。
#-Wall:
This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros.幾乎包含了一般情況該警告的事項。
#-Wextra:
This enables some extra warning which are not enabled by -Wall.
#-fPIC:
If supported for the target machine, it will generate position-independent code suitable for use in a shared library and dynamic linking and avoiding any limit on the size of the global offset table.之所以要產生 position-independent code也就是產生的code只有相對地址沒有絕對位置，是因為shared library  被加載時內存的位置不固定，因此需要code可以被加載到內存的任意位置都可以正確執行。
#-I : adds include directory of header files. 
#-shared: 
Produce a shared object which can then be linked with other objects to form an executable.
#-o:
Write the build output to an output file.


另外，是我們補充的其他編譯指令解釋:
#-Werror          : regard warring as error 會將所有警告都轉成錯誤，讓編譯停止
#-Wextra          : print extra warring hint
#-pedantic-errors : follow ANSI standard, otherwise, turn warring to error
```
## 請解釋 Game.h 裡面 ```call``` 函數的功能 (5%)
```c
template<typename Func ,typename... Args, 
    std::enable_if_t< std::is_void<
    std::invoke_result_t<Func, AIInterface, Args...>
    >::value , int> = 0 >
void call(Func func, AIInterface *ptr, Args... args)
{
    std::future_status status;
    /*
    future_status
    包含在future標準函式庫
    future_status 後面的 status 是個 Enumeration
    共有三種 Enumerator ， 分別是:
    1. ready     值為0
    2. timeout   值為1
    3. deferred  值為2
    */
    auto val = std::async(std::launch::async, func, ptr, args...);
    /*
    val 會接收來自 std::async(std::launch::async, func, ptr, args...) 的資料
    asnyc 包含在 future 函式庫
    std::launch::async 是一種執行策略，會使 func 在新的線程執行
    func 是 ptr 裡的函數
    ptr 可以是一個物件的指標 func 則是其成員函數
    args 則是要給func 的參數
    但因為每個 func 所需要接收的參數個數可能不同
    args...這種寫法則可以解決以上問題，增加彈性
    */
    status = val.wait_for(std::chrono::milliseconds(m_runtime_limit));
    /*
    wait_for函數等待 val 接收到值，並設置時限為1000毫秒
    */
    if( status != std::future_status::ready )
    {
        exit(-1);
    }
    /*
    如果狀態不為ready (值沒有在1000ms 內接收到) ， 離開程式
    */
    val.get();
    /*
    取得 val 的值
    */
}
```
上面這個call 函式是不會回傳值得
因此另外還有個call 函式
只差了一行
```c
return val.get()  
```
會把值傳回來
## 請解釋什麼是 Shared library，為何需要 Shared library ，在 Windows 系統上有類似的東西嗎？(10%)

1. Shared library 是一個包含許多 ```object code``` 的檔案。

2. 有了shared library ， 編譯專案所產生出來的執行檔會比較小，因為執行檔和 library 是分離的，不像 static library， 把所有的程式都包在執行檔中。也因為如此，當 static library 有函式庫要更新的話，勢必就要連同執行檔也一起更新，然而 share library 卻不用。這樣的好處會在專案體積特別龐大的時候有顯著的好處。例如這一次的作業分工，在程式碼合併之前可能就要自己先編譯並測試好幾次才行。但程式碼更改的範圍通成都侷限在小範圍內，有了shared library，就能加快編譯速度。

3. 在 windows 系統裡沒有 shared library 


# Final Project

## AI Algorithm (10%)

( 請說明 AI 如何運作，進攻/防守的策略是什麼 )

## 分工與進度規劃 (5%)

( 請說明團隊上的分工，每個人做了什麼，如果有進度規劃也請提供 )
```
分工:
黃文遠: AI 版本2 、gamerunner 
廖凰汝: gamerunner
陳弘輊：　AI 版本1
進度規劃：
6/1：　進度規劃
6/2~6/7: 看懂code 理解大致上要做甚麼
6/8: 決定分工，並開github，fork
6/19~6/21: 各自寫好code，問題討論，互相追蹤進度
6/22: gamerunner merge，修好一些bug，gamerunner 完成
6/23: AI　做些修改，生出用dfs做的版本二，整體完成
```

## 心得 (10%) 每人都要寫，不少於 100 字，Ex. 實作時有遇到什麼困難，如何排除等等。


