# Custom Intepreter project

This is an Interpreter built in C for a custom programming language named "SimpleScript". The Interpreter is designed to recognise variable assignent, integer and string types, comments and output functionality. Future functionality includes simple mathematical statements, user input and functions. 

## 🌐 Progress:

- Lexical analysis 🟦🟦🟦🟦🟦🟦🟦🟦🟦🟦 100%
- Syntax analysis  🟦🟦🟦🟦🟦🟦🟦🟦⬜️⬜️ 80%
- Semantic analysis ⬜️⬜️⬜️⬜️⬜️⬜️⬜️⬜️⬜️⬜️ 0%

## 🌐 Language Description 

```
Comments: @
File extension: .ss
Language description:  simple, imperative, procedural scripting language
```

## 🌐 Grammar rules

```
assignment: <variable> <type> ':' <value>
output: "Output" '{' (<value> (',' <value>)*)? '}'
comment: '@' <text>
variable: <identifier>
type: "Int" | "String"
value: <number> | <string_literal>
identifier: <letter> (<letter> | <digit>)*
number: <digit>+
string_literal: "'" <string_content> "'"
string_content: <character>*
text: <character>*
letter: [a-zA-Z]
digit: [0-9]
character: any valid character except for single quote
```

## 🌐 Example Code:

```
Value1(Int): 100
Value2(String): 'Hello'
Output{Value1}
Output{Value1,Value2}
Output{'this is a test'}
@ comment
Output{'enter your age'}

```


# 📝 Contributing
Your contributions are always welcome. For major revisions, please start by opening an issue to discuss what you'd like to change.

# 📜 License
This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).

# 💼 Contact
For any inquiries, suggestions, or feedback, don't hesitate to email me at [ostrynskimaks@gmail.com](mailto:ostrynskimaks@gmail.com).
We encourage suggestions for improvements and new features!
