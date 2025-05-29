import 'package:fl_chart/fl_chart.dart';
import 'package:flutter/material.dart';

class MeasurementChartScreen extends StatelessWidget {
  final String title;
  final String value;
  final List<double> data;
  final Color lineColor;

  const MeasurementChartScreen({
    super.key,
    required this.title,
    required this.value,
    required this.data,
    required this.lineColor,
  });

  @override
  Widget build(BuildContext context) {
    final minY = data.reduce((a, b) => a < b ? a : b) - 5;
    final maxY = data.reduce((a, b) => a > b ? a : b) + 5;

    return Scaffold(
      backgroundColor: const Color(0xFFFFF1F2), // soft pink
      appBar: AppBar(
        backgroundColor: const Color(0xFFFFF1F2),
        elevation: 0,
        leading: const BackButton(color: Colors.black),
        title: const Text(
          'VITALTRACK',
          style: TextStyle(
            color: Colors.black,
            letterSpacing: 1.5,
            fontWeight: FontWeight.bold,
          ),
        ),
        centerTitle: true,
      ),
      body: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Text(
              title,
              style: const TextStyle(
                fontSize: 32,
                fontWeight: FontWeight.w700,
                color: Colors.black,
              ),
            ),
            const SizedBox(height: 4),
            Text(
              value,
              style: const TextStyle(
                fontSize: 28,
                fontWeight: FontWeight.w500,
                color: Colors.black,
              ),
            ),
            const SizedBox(height: 16),
            SizedBox(
              height: 200,
              child: LineChart(
                LineChartData(
                  minY: minY,
                  maxY: maxY,
                  gridData: FlGridData(
                    show: true,
                    drawVerticalLine: false,
                    horizontalInterval: 10,
                    getDrawingHorizontalLine: (value) => FlLine(
                      color: Colors.grey.withOpacity(0.2),
                      strokeWidth: 1,
                    ),
                  ),
                  titlesData: FlTitlesData(
                    show: true,
                    bottomTitles: AxisTitles(
                      sideTitles: SideTitles(
                        showTitles: false,
                      ),
                    ),
                    leftTitles: AxisTitles(
                      sideTitles: SideTitles(
                        showTitles: true,
                        reservedSize: 40,
                        getTitlesWidget: (value, meta) => Text(
                          value.toInt().toString(),
                          style: const TextStyle(fontSize: 12, color: Colors.grey),
                        ),
                      ),
                    ),
                    topTitles: AxisTitles(
                      sideTitles: SideTitles(showTitles: false),
                    ),
                    rightTitles: AxisTitles(
                      sideTitles: SideTitles(showTitles: false),
                    ),
                  ),
                  borderData: FlBorderData(show: false),
                  lineBarsData: [
                    LineChartBarData(
                      isCurved: true,
                      color: lineColor,
                      barWidth: 4,
                      dotData: FlDotData(show: false),
                      belowBarData: BarAreaData(show: false),
                      spots: List.generate(
                        data.length,
                            (i) => FlSpot(i.toDouble(), data[i]),
                      ),
                    )
                  ],
                ),
              ),
            ),
            const SizedBox(height: 24),
            Center(
              child: Container(
                decoration: BoxDecoration(
                  color: const Color(0xFFFFE5E7),
                  borderRadius: BorderRadius.circular(16),
                ),
                child: ToggleButtons(
                  borderRadius: BorderRadius.circular(16),
                  fillColor: const Color(0xFFFFF1F2),
                  selectedColor: Colors.black,
                  color: Colors.grey,
                  isSelected: const [true, false, false],
                  onPressed: (_) {},
                  children: const [
                    Padding(
                      padding: EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                      child: Text("Day", style: TextStyle(fontSize: 16)),
                    ),
                    Padding(
                      padding: EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                      child: Text("Week", style: TextStyle(fontSize: 16)),
                    ),
                    Padding(
                      padding: EdgeInsets.symmetric(horizontal: 16, vertical: 8),
                      child: Text("Month", style: TextStyle(fontSize: 16)),
                    ),
                  ],
                ),
              ),
            ),
            const Spacer(),
          ],
        ),
      ),
    );
  }
}
