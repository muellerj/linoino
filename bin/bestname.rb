#!/usr/bin/env ruby

path = ARGV.shift
dir  = File.dirname(path)
name = File.basename(path)

ALLOWED_CHARS="a-zA-z0-9\-_."

basename = name.
  downcase.
  gsub("ä", "ae").
  gsub("ö", "oe").
  gsub("ü", "ue").
  gsub("ß", "ss").
  gsub(/[^a-z0-9\-_.]/, "_")

i = (basename[/^\d+/] || 1).to_i

basename.gsub!(/^\d+[\-_]*/, "")

name = sprintf("%04d", i) + "_" + basename

while File.exist?(File.join(dir, name))
  i += 1
  name = sprintf("%04d", i) + "_" + basename
end

puts File.join(dir, name)
