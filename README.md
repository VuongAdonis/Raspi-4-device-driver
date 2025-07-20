# Raspi-4-device-driver

RUle of code:

1. Follow Consistent Naming Conventions

- Use descriptive and meaning ful names fr variables, functions, classes, and constants.

- Use camelCase or snake_case consistently according to your language's convention.

- Avoid abbreviations unless they are well-known.

2. Keeping Functions and Methods Small and Focused.

- Each function/method should do one thing and do it well.

- Limit the length of functions to improve readability and maintainability.

3. Use Proper Indentation and Spacing.

- Follow the indentation rulesof your language.

- Use blank lines to separate logical blocks of code.

- Keep line length within readable limits (usuall 80-120 characters).

4. Aviod Magic Number and Strings

- Use named constants or enums instead of hardcoding numbers of strings.

5. Handle Errors and Exceptions Properly.

- Use try/catch or equivalent where necessary.

- Avoid empty catch blocks.

- Provide meaningful error messages.

6. Remove Dead Code and Unused Imports/Variables.

- Delete code that is no longer used.

- Avoid redundant code duplication.

7. Comment and Document Wisely.

- Use comments to explain why something is done, not what is done.

- Write docstrings for functions/classes with inputs, outputs and purpose.

8. Avoid Code Smells.

- Detect and refactor repeated code (DRY principle).

- Avoid deep nesting by erly returns.

- Avoid large classes and methods.

9. Use Language-Specific code Style Guides.

- Cú pháp dấu cách: luôn có một space giữa từ khóa điều khiển (if, while, for, switch) và dấu mở ngoặc đơn: if (condition)

- Tên biến, hàm, kiểu nên dùng chữ thường, có thể dùng dấu gạch dưới _ để phân tách.

- Không dùng prefix với "__" hoặc "_" ở đầu tên biến, hàm, macro, vì đây là dành cho compiler hoặc thư viện chuẩn.

- Ký tự mở ngoặc nhọn { đặt ở cùng dòng với từ khóa (if, for, ...).

10. Pass All Static Analysis Warnings and Errors.