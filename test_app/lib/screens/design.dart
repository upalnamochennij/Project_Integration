import 'package:flutter/material.dart';
import '../functions/vital_trend.dart';
import 'measurement_chart_screen.dart';
import '../widgets/vital_card.dart';

class VitalMainScreen extends StatelessWidget {
  const VitalMainScreen({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFFFFF4F2),
      body: Column(
        children: [
          Container(
            width: double.infinity,
            padding: const EdgeInsets.symmetric(vertical: 20),
            color: const Color(0xFFFFDADC),
            child: const SafeArea(
              bottom: false,
              child: Center(
                child: Text(
                  'VITALWATCH',
                  style: TextStyle(
                    fontSize: 20,
                    fontWeight: FontWeight.bold,
                    letterSpacing: 1.5,
                    color: Colors.black87,
                  ),
                ),
              ),
            ),
          ),
          Expanded(
            child: SafeArea(
              top: false,
              child: Padding(
                padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 20),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: [
                    const Text(
                      'Welcome to\nVitalWatch 🩺',
                      style: TextStyle(fontSize: 30, fontWeight: FontWeight.w700, height: 1.2),
                    ),
                    const SizedBox(height: 12),
                    const Text(
                      'Your latest measurements',
                      style: TextStyle(fontSize: 16, color: Colors.black54),
                    ),
                    const SizedBox(height: 24),
                    VitalCard(
                      icon: Icons.favorite,
                      label: 'Heart Rate',
                      value: '72 bpm',
                      color: Colors.red,
                      bgColor: const Color(0xFFFFE5E7),
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (_) => const MeasurementChartScreen(
                              title: 'Heart Rate',
                              value: '72 bpm',
                              data: [65, 70, 68, 75, 69, 77, 72, 80],
                              lineColor: Colors.red,
                            ),
                          ),
                        );
                      },
                    ),
                    VitalCard(
                      icon: Icons.directions_walk,
                      label: 'Steps',
                      value: '6,200 steps',
                      color: Colors.blue,
                      bgColor: const Color(0xFFD6ECF8),
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (_) => const MeasurementChartScreen(
                              title: 'Steps',
                              value: '6,200 steps',
                              data: [4000, 5000, 6200, 5800, 6100, 6700, 6900],
                              lineColor: Colors.blue,
                            ),
                          ),
                        );
                      },
                    ),
                    VitalCard(
                      icon: Icons.thermostat,
                      label: 'Temperature',
                      value: '98.6°F',
                      color: Colors.deepOrange,
                      bgColor: const Color(0xFFFFECD4),
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (_) => const MeasurementChartScreen(
                              title: 'Temperature',
                              value: '98.6°F',
                              data: [98.0, 98.3, 98.6, 98.7, 98.4, 98.8],
                              lineColor: Colors.orange,
                            ),
                          ),
                        );
                      },
                    ),
                    VitalCard(
                      icon: Icons.opacity,
                      label: 'Blood Oxygen',
                      value: '98%',
                      color: Colors.teal,
                      bgColor: const Color(0xFFE4F6EE),
                      onTap: () {
                        Navigator.push(
                          context,
                          MaterialPageRoute(
                            builder: (_) => const MeasurementChartScreen(
                              title: 'Blood Oxygen',
                              value: '98%',
                              data: [96, 97, 98, 97, 99, 98],
                              lineColor: Colors.teal,
                            ),
                          ),
                        );
                      },
                    ),
                    const Spacer(),
                    Center(
                      child: TextButton.icon(
                        style: TextButton.styleFrom(
                          backgroundColor: const Color(0xFFFFE5E7),
                          padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 12),
                          shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(16),
                          ),
                        ),
                        onPressed: () {
                          Navigator.push(
                            context,
                            MaterialPageRoute(
                              builder: (_) => const VitalTrend(),
                            ),
                          );
                        },
                        label: const Text(
                          'History',
                          style: TextStyle(color: Colors.red, fontSize: 20),
                        ),
                      ),
                    ),
                    const SizedBox(height: 20),
                  ],
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
