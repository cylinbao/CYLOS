file kernel/system
target remote localhost:1234

def link
	target remote localhost:1234
end

def sn
	stepi
	info r
end
