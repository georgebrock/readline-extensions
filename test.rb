require 'readline'
require './readlineextensions'

Readline.completion_proc = Proc.new do |input|
  options = [ 'some\ file.txt', 'some\ other\ file.txt' ]
  options.select { |option| option.start_with?(input) }
end

Readline.completer_quote_characters = '"\'\\'
Readline.completer_word_break_characters = " \t\n\"'"

ReadlineExtensions.quoting_detection_proc = Proc.new do |text, index|
  index > 0 && text[index-1] == '\\'
end

while line = Readline.readline('> ', true)
  p line
end

print "\n"
