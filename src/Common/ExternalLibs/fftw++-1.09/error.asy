include graph;

size(175,200,IgnoreAspect);

scale(Log,Log(true,true));
real[] mp,p,mu,u,mP,P;

string name;

usersetting();

if(name == "") name=getstring("program name");

string dir;
if(name == "conv") dir="timings1r/error.";
else if(name == "cconv") dir="timings1c/error.";
else abort("error test not implemented for "+name);

file fin=input(dir+"explicit").line();
real[][] a=fin.dimension(0,0);
a=transpose(a);
mp=a[0]; p=a[1];

file fin=input(dir+"implicit").line();
real[][] a=fin.dimension(0,0);
a=transpose(a);
mu=a[0]; u=a[1];

guide g0=scale(0.5mm)*unitcircle;
guide g1=scale(0.6mm)*polygon(4);

monoPen[0]=dashed;
monoPen[1]=solid;

marker mark0=marker(g0,Draw(Pen(0)+solid));
marker mark1=marker(g1,Draw(Pen(1)+solid));

pen lp=fontsize(8pt);
draw(graph(mp,p,p>0),Pentype(0),Label("explicit",Pen(0)+lp),mark0);
draw(graph(mu,u,u>0),Pentype(1),Label("implicit",Pen(1)+lp),mark1);

xaxis("$m$",BottomTop,LeftTicks);
yaxis("normalized error",LeftRight,RightTicks);

legendlinelength=0.6cm;
legendmargin=5;
attach(legend(),point(NW),17SE);
