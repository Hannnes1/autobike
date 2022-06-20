% Read all of the data in the TDMS file.
data = tdmsread("Logs/Test3/test6P5.tdms");

%% Encoder

% `data` contains one cell per group. Each cell contains
% a table with one column per channel. In this case, the
% first channel in the first group contains the time, 
% and the second channel contains the position etc.
time1 = table2array(data{1,1}(:,1));
position = table2array(data{1,1}(:,2));
velocity = table2array(data{1,1}(:,3));

figure;
%yyaxis left;
% 611 is the conversion ratio from counts on the encoder
% to degrees.
plot(time1-time1(1),position/611);
ylim([-10,10]);
ylabel("Position (degrees)");
hold on;
%yyaxis right;
%plot(time1-time1(1),velocity/611*1000);
%ylim([-50,50]);
%ylabel("Velocity (degrees/s)");
xlabel("Time (seconds)");
xlim([22,30]);
title("Steering motor readings");
hold off;

%% Gyro
time2 = table2array(data{1,2}(:,1));
X = table2array(data{1,2}(:,2));
Y = table2array(data{1,2}(:,3));
Z = table2array(data{1,2}(:,4));

figure;
hold on;
plot(time2-time2(1),X);
plot(time2-time2(1),Y);
plot(time2-time2(1),Z);
ylabel("Angular velocity (rad/s)");
xlabel("Time (seconds)");
title("Gyro");
%yyaxis right;

angle = zeros(length(Z),1);
for i = 1:length(Z) - 1
    angle(i + 1) = angle(i) + (time2(i+1) - time2(i)) * Z(i);
end
%plot(time2-time2(1),angle);
%ylabel("Lean angle (radians)");
legend("X", "Y", "Z", "Lean angle");

%% Steering motor

time3 = table2array(data{1,3}(:,1));
pwm = table2array(data{1,3}(:,2));

figure;
plot(time3-time3(1),pwm);
ylim([0,1]);
ylabel("Duty cycle");
xlabel("Time (seconds)");
title("Steering motor control");

%% Combined

figure;
yyaxis left;
plot(time3-time2(1),pwm);
ylim([0,1]);
ylabel("Duty cycle");
hold on;
yyaxis right;
plot(time2-time2(1),Z);
ylabel("Roll rate (rad/s)");
ylim([-1.5,1.5]);
xlim([22,30]);
xlabel("Time (seconds)");
title("Duty cycle and Roll rate");

%% PWM vs angular velocity

pwm01 = tdmsread("Logs/pwm01.tdms");
pwm02 = tdmsread("Logs/pwm02.tdms");
pwm03 = tdmsread("Logs/pwm03.tdms");
pwm04 = tdmsread("Logs/pwm04.tdms");

time01 = table2array(pwm01{1,1}(:,1));
velocity01 = table2array(pwm01{1,1}(:,3));
position01 = table2array(pwm01{1,1}(:,2));

time02 = table2array(pwm02{1,1}(:,1));
velocity02 = table2array(pwm02{1,1}(:,3));

time03 = table2array(pwm03{1,1}(:,1));
velocity03 = table2array(pwm03{1,1}(:,3));

time04 = table2array(pwm04{1,1}(:,1));
velocity04 = table2array(pwm04{1,1}(:,3));

%%

figure;
hold on;
plot(time01 - time01(1) - 1.56,velocity01/611*1000);
plot(time02 - time02(1) - 0.9,velocity02/611*1000);
plot(time03 - time03(1) - 1.59,velocity03/611*1000);
plot(time04 - time04(1) - 0.78,velocity04/611*1000);
xlabel("Time (seconds)");
ylabel("Velocity (degrees/second)");
xlim([-1,9]);
legend("duty cycle = 0.1", "duty cycle = 0.2", "duty cycle = 0.3", "duty cycle = 0.4", 'Location','northwest');
title("Relation between duty cycle and angular velocity");
