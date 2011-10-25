MAX_TIME = 2 #if the computation takes less than that (in seconds), increases the precision for that specific w

def compute_to_good_precision(w, precision, max_precision)
	while true
		start_time = Time.now
		result = `./cylinder #{w} #{precision}`
		end_time = Time.now
		return result if (end_time - start_time > 2 or precision >= max_precision)
		precision = precision + 1
	end
end


widths = (3..15).to_a
initial_precision = 10

results = widths.collect{|w| puts "now computing for w=#{w}..."; [w,compute_to_good_precision(w,initial_precision,30)]}

results.each{|result| puts "#{result.first}: #{result.last}"}