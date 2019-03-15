//most code taken from lecture slide (tempalte)

module xor8 (xout, xin1, xin2); output [7:0] xout;
 input [7:0] xin1,xin2;

xor(xout[7], xin1[7], xin2[7]),
(xout[6], xin1[6], xin2[6]),
(xout[5], xin1[5], xin2[5]),
(xout[4], xin1[4], xin2[4]),
(xout[3], xin1[3], xin2[3]),
(xout[2], xin1[2], xin2[2]),
(xout[1], xin1[1], xin2[1]),
(xout[0], xin1[0], xin2[0]);

endmodule

module deMux (outVector, a, b, c, d, enable); input [7:0] outVector;
input	a, b, c, d, enable;
wire	m11, m10, m8, m6, m5, m4, m2; and
(m11, ~a, ~b, c, d, enable),
(m10,  a,  b, ~c,  d, enable),
(m9,  ~a,  b, ~c,  d, enable),	
(m8,   a, ~b, ~c,  d, enable),
(m6, a, b, c, ~d, enable),
(m5,  ~a,  b,  c, ~d, enable),
(m4,   a, ~b,  c, ~d, enable),
(m2,   a,  b, ~c, ~d, enable);
assign outVector = {m11, m10, m9, m8, m6, m5, m4, m2};

endmodule

module hamDecode (vIn, valueOut, error); input [12:0] vIn;
output [7:0] valueOut; output error;
wire	c1, c2, c4, c8;
wire [7:0]	bitFlippers;

xor	(c1, vIn[0], vIn[2], vIn[4], vIn[6], vIn[8], vIn[10]),
(c2, vIn[1], vIn[2], vIn[5], vIn[6], vIn[9], vIn[10]),
(c4, vIn[3], vIn[4], vIn[5], vIn[6], vIn[11]),
(c8, vIn[7], vIn[8], vIn[9], vIn[10], vIn[11]);

deMux mux1 (bitFlippers, c1, c2, c4, c8, 1'b1);
xor8 x1 (valueOut, bitFlippers, {vIn[11], vIn[10], vIn[9], vIn[8],
vIn[6], vIn[5], vIn[4], vIn[2]});
assign error = ~^vIn & (c1 | c2 | c4 | c8); endmodule

module hamEncode (vIn, valueOut); input [7:0] vIn;
output [12:0]  valueOut; wire	h1, h2, h4, h8, hA; wire [11:0]	preParity;
xor	(h1, vIn[0], vIn[1], vIn[3], vIn[4], vIn[6]),
(h2, vIn[0], vIn[2], vIn[3], vIn[5], vIn[6]),
(h4, vIn[1], vIn[2], vIn[3], vIn[7]),
(h8, vIn[4], vIn[5], vIn[6], vIn[7]);
assign preParity = {vIn[7:4], h8, vIn[3:1], h4, vIn[0], h2, h1}; assign hA = ^preParity;
assign valueOut = {hA, preParity}; endmodule

module testHam(); reg [7:0] original;
wire [7:0] regenerated;
wire [12:0] encoded, messedUp; wire	error;
reg [3:0] messUp;
reg [3:0] messUpAgn; integer	seed;
initial begin seed = 53; forever begin
original = $random(seed); messUp = $random(seed);
messUpAgn = $random(seed); #1
if ((messUp <= 12 || messUpAgn <= 12) && (messUp != messUpAgn))
$display("original=%h, encoded=%h, messer=%4h,messed=%h, error=%b, regen=%h",original, encoded, (2 ** messUp) ^ (2 ** messUpAgn), messedUp, error, regenerated);
// if it isn't working correctly
if ((original != regenerated) && ~error)
$fatal("original does not match regenerated"); end // forever begin
end // initial begin

hamEncode hIn (original, encoded);
hamDecode hOut (messedUp, regenerated, error);

assign messedUp = encoded ^ (2 ** messUp) ^ (2 ** messUpAgn);

endmodule
 
//most code taken from lecture slides (template)
