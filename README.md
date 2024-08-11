# Try Invoke callbacks

## 示例

```cpp
auto rt = try_invoke([](int a, int b){return a + b;},
    3, 
    4, 
    std::tuple{3, 4}
) // rt 的值为 7
```
```cpp
try_invoke([]{ dosomething(); }, 
    ju::void_t,
    //... 其他可能传入的参数
)
```
例子中的3， 4， (3, 4) ，void_t 会被依次尝试作为参数传入回调函数，  
其中void_t指函数无参数