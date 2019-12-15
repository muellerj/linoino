#!/usr/bin/env ruby

require "rake"

def bestname(path)
  dir  = File.dirname(path)
  name = File.basename(path)

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

  File.join(dir, name)
end

path = ARGV.shift

files = Dir.glob(File.join(path, "*.m4a"))

files.sort.each do |file|
  newname = bestname(file.ext(".mp3"))
  puts "Converting #{file} -> #{newname}"
  system "ffmpeg", \
    "-i", file, \
    "-acodec", "libmp3lame", \
    "-aq", "2", \
    newname
end

