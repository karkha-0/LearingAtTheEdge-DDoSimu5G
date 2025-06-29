

package ddosim5g.MessageSender;

simple MessageSender {
	parameters:
	    @class(MessageSender);
	   	int startPredictTime = default(10);
	   	
	   	
	   	@signal[predictEventTime];
	   	@statistics[predictEventTime];
	   	
	
}
