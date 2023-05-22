var std = jsarguments[1] || 1;
var ctr = jsarguments[2] || 0;



function bang() 
{
  var a = Math.random();
  var b = Math.random();
  var z1 = Math.sqrt(-2 * Math.log(a)) * Math.cos(2 * Math.PI * b);
  var out = z1 * std + ctr;
  outlet(0, out)
}
	
	