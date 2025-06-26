#!/bin/bash

# DreamLang V2 演示脚本

echo "DreamLang V2 编译器演示"
echo "======================"
echo ""

# 确保在正确的目录
cd "$(dirname "$0")"

# 检查可执行文件是否存在
if [ ! -f "build/dreamlang" ]; then
    echo "错误: 找不到 build/dreamlang 可执行文件。"
    echo "请先运行 ./build.sh 来构建项目。"
    exit 1
fi

DREAMLANG="./build/dreamlang"

echo "1. 显示版本信息（英文）"
echo "命令: $DREAMLANG --version"
$DREAMLANG --version
echo ""

echo "2. 显示版本信息（中文）"
echo "命令: $DREAMLANG -l zh_CN --version"
$DREAMLANG -l zh_CN --version
echo ""

echo "3. 显示帮助信息（英文）"
echo "命令: $DREAMLANG --help"
$DREAMLANG --help
echo ""

echo "4. 显示帮助信息（中文）"
echo "命令: $DREAMLANG -l zh_CN --help"
$DREAMLANG -l zh_CN --help
echo ""

echo "5. 分析正常的 DreamLang 源文件（英文）"
echo "命令: $DREAMLANG examples/calculator.zv"
$DREAMLANG examples/calculator.zv
echo ""

echo "6. 分析正常的 DreamLang 源文件（中文）"
echo "命令: $DREAMLANG -l zh_CN examples/calculator.zv"
$DREAMLANG -l zh_CN examples/calculator.zv
echo ""

echo "7. 测试自动添加 .zv 后缀功能"
echo "命令: $DREAMLANG examples/calculator (自动添加 .zv)"
$DREAMLANG examples/calculator
echo ""

echo "8. 显示详细的词法分析结果（前15行）"
echo "命令: $DREAMLANG -t examples/calculator.zv | head -15"
$DREAMLANG -t examples/calculator.zv | head -15
echo "    ..."
echo ""

echo "9. 错误报告演示（英文）"
echo "命令: $DREAMLANG examples/simple_error.zv"
$DREAMLANG examples/simple_error.zv || true
echo ""

echo "10. 错误报告演示（中文）"
echo "命令: $DREAMLANG -l zh_CN examples/simple_error.zv"
$DREAMLANG -l zh_CN examples/simple_error.zv || true
echo ""

echo "11. 自动后缀的错误报告演示"
echo "命令: $DREAMLANG -l zh_CN examples/simple_error (自动添加 .zv)"
$DREAMLANG -l zh_CN examples/simple_error || true
echo ""

echo "12. 使用环境变量设置语言"
echo "命令: LANG=zh_CN.UTF-8 $DREAMLANG examples/simple_error"
LANG=zh_CN.UTF-8 $DREAMLANG examples/simple_error || true
echo ""

echo "演示完成！"
echo ""
echo "关键特性总结:"
echo "- ✓ 完整的 DreamLang 词法分析器"
echo "- ✓ 国际化支持（中文/英文）"
echo "- ✓ 本地化错误报告"
echo "- ✓ 命令行语言切换"
echo "- ✓ 环境变量自动检测"
echo "- ✓ 详细的 Token 分析"
echo "- ✓ .zv 文件扩展名支持"
echo "- ✓ 自动添加 .zv 后缀功能"
