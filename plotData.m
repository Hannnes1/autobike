data = tdmsread("log5P3S45.tdms");

%% Encoder
time1 = table2array(data{1,1}(:,1));
position = table2array(data{1,1}(:,2));
velocity = table2array(data{1,1}(:,3));

figure;
plot(time1-time1(1),position/611);
ylim([-90,90]);
ylabel("Position (degrees)");
hold on;
yyaxis right;
plot(time1-time1(1),velocity/611);
ylim([-1,1]);
ylabel("Velocity (degrees/ms)");
xlabel("Time (seconds)");
title("Steering motor readings");
hold off;

%% Gyro
time2 = table2array(data{1,2}(:,1));
rollRate = table2array(data{1,2}(:,2));

figure;
plot(time2-time2(1),rollRate);
ylabel("Roll rate");
xlabel("Time (seconds)");
title("Gyro");

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
plot(time2-time2(1),rollRate);
ylabel("Roll rate");
ylim([-10,10]);
xlabel("Time (seconds)");
title("Roll rate compared to PWM");










