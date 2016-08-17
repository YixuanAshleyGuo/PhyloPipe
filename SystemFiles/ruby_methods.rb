
def mda(width,height)
	a = Array.new(width)
	a.map! { Array.new(height) }
	return a
end

def openfile(filename,rw)
	begin
	file = File.open(filename,rw)
	rescue
    		STDERR.print "Could not open file #{filename}!\n"
    		exit 1
	end
return file
end



def openfasta(filename)
	begin
	fastafile = Bio::FastaFormat.open(filename)
	rescue
    		STDERR.print "Could not open file #{filename}!\n"
    		exit 1
	end
return fastafile
end


def name2id(file, taxname)
	file.pos = 0
	while (line = file.gets)
		tax_arr = line.split("\t|")		
		if tax_arr[1]==taxname && (tax_arr.include?("\tscientific name"))
			return tax_arr[0]
		end
	end
	STDERR.puts "Cannot find taxon name!"
end


def id2name(file, id)
	file.pos = 0
	while (line = file.gets)
		tax_arr = line.split("\t|")
		if (tax_arr[0] == id) && (tax_arr.include?("\tscientific name"))
			tax_arr[1].gsub!(/\t/ , "")
			return tax_arr[1]
		end
	end
	STDERR.puts "Cannot find taxon id!"
end




def family_id_recursion(taxonomy, id)
	parent, rank = taxonomy[id]
	if rank == "family"
		return id
	else
		return family_id_recursion(taxonomy, parent)
	end
end	




def arthro_id_recursion(taxonomy, id)

	parent, rank = taxonomy[id]
	
	if parent == "6843"
		return "6843"
	elsif parent == "6657"
		return "6657"
	elsif parent == "6960"
		return "6960"
	elsif parent == "61985"
		return "61985"
	elsif parent == "1"
		return "1"
	else
		return arthro_id_recursion(taxonomy, parent)
	end
end	





