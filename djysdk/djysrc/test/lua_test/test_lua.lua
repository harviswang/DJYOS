	Lmyputs0("** start! **\10",6); 
	Lsleep(1000); 
	a=0;
	week={"Sunday","Monday","Tuesday","Wednesday",
				"Thursday","Friday","Saturday"};

	Lmyputs0("-----------\10",8);

	while 1>0 do
	
		a =(a%7)+1;
	
		Lmyputs0(week[a],8);
		Lmyputs0("\10",8);
		Lsleep(500);
		
	end

	Lsleep(1000); 