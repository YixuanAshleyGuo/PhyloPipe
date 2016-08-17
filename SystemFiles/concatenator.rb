#!/usr/bin/env ruby

require 'bio'
require "ruby_methods"

USAGE = "#{$0} directory outfile_msa outfile_charset"

if ARGV.size<3
    STDERR.puts USAGE
    exit 1
end

hash = Hash.new

num = ARGV.size

dir = Dir.new(ARGV[0])

dir.each do |filename|

	if filename.match(/.+\.fa/)
		file = openfasta(filename)
		file.each do |entry|
			if not hash.has_key?(entry.definition)
				hash[entry.definition] = ""
			end
		end
		file.close
	end
end

current_size 	= 0
absolute_size	= 1
seq_size	= 0

file2 = openfile(ARGV[2],"w")

dir.each do |filename|

	if filename.match(/.+\.fa/)
		file  = openfasta(filename)

		file.each do |entry|
				hash[entry.definition] += entry.seq
				seq_size	= entry.seq.size	
		end

		file2.puts "charset  #{filename}  =  #{absolute_size} - #{(absolute_size-1)+seq_size} ;"
		absolute_size += seq_size
		file.close

		hash.each_key do |key|
			if hash[key].size < (absolute_size-1)
				hash[key] += "X"*seq_size
			end
		end
	end
end

file2.close

out = openfile(ARGV[1],"w")

hash.each_key do |key|
	out.puts ">" + key
	out.puts hash[key]
end

out.close
