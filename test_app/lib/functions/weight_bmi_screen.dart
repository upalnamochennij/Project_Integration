import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';

class WeightBMIScreen extends StatefulWidget {
  const WeightBMIScreen({super.key});

  @override
  State<WeightBMIScreen> createState() => _WeightBMIScreenState();
}

class _WeightBMIScreenState extends State<WeightBMIScreen> {
  final TextEditingController weightController = TextEditingController(
    text: "60",
  );
  final TextEditingController heightController = TextEditingController(
    text: "160",
  );

  double? _bmi;

  void _calculateBMI() {
    final weight = double.tryParse(weightController.text);
    final height = double.tryParse(heightController.text);

    if (weight != null && height != null && height > 0) {
      final heightInMeters = height / 100;
      setState(() {
        _bmi = weight / (heightInMeters * heightInMeters);
      });
    }
  }

  void _showPicker(BuildContext context, {required bool isWeight}) {
    final List<int> values = List.generate(
      200,
      (i) => i + (isWeight ? 20 : 100),
    );
    showModalBottomSheet(
      context: context,
      builder: (_) {
        return SizedBox(
          height: 250,
          child: CupertinoPicker(
            itemExtent: 40,
            scrollController: FixedExtentScrollController(
              initialItem: values.indexOf(
                int.tryParse(
                      isWeight ? weightController.text : heightController.text,
                    ) ??
                    (isWeight ? 60 : 160),
              ),
            ),
            onSelectedItemChanged: (index) {
              setState(() {
                if (isWeight) {
                  weightController.text = values[index].toString();
                } else {
                  heightController.text = values[index].toString();
                }
              });
            },
            children: values.map((v) => Center(child: Text("$v"))).toList(),
          ),
        );
      },
    );
  }

  Widget _buildBMIGauge(double bmi) {
    return SfRadialGauge(
      axes: <RadialAxis>[
        RadialAxis(
          minimum: 10,
          maximum: 40,
          ranges: <GaugeRange>[
            GaugeRange(startValue: 10, endValue: 18.5, color: Colors.blue),
            GaugeRange(startValue: 18.5, endValue: 25, color: Colors.green),
            GaugeRange(startValue: 25, endValue: 30, color: Colors.orange),
            GaugeRange(startValue: 30, endValue: 40, color: Colors.red),
          ],
          pointers: <GaugePointer>[NeedlePointer(value: bmi)],
          annotations: <GaugeAnnotation>[
            GaugeAnnotation(
              widget: Text(
                bmi.toStringAsFixed(1),
                style: const TextStyle(
                  fontSize: 20,
                  fontWeight: FontWeight.bold,
                ),
              ),
              angle: 90,
              positionFactor: 0.5,
            ),
          ],
        ),
      ],
    );
  }

  String _bmiCategory(double bmi) {
    if (bmi < 18.5) return "Underweight";
    if (bmi < 25) return "Normal";
    if (bmi < 30) return "Overweight";
    return "Obese";
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0xFFFFF4F2),
      appBar: AppBar(
        title: const Text(
          "BMI Tracker",
          style: TextStyle(fontWeight: FontWeight.bold),
        ),
        backgroundColor: const Color(0xFFFFDADC),
        centerTitle: true,
      ),
      body: Padding(
        padding: const EdgeInsets.all(20),
        child: ListView(
          children: [
            const Text(
              "Enter your Weight & Height",
              style: TextStyle(fontSize: 18, fontWeight: FontWeight.w600),
            ),
            const SizedBox(height: 20),
            TextField(
              controller: weightController,
              keyboardType: TextInputType.number,
              decoration: InputDecoration(
                labelText: "Weight (kg)",
                border: const OutlineInputBorder(),
                suffixIcon: IconButton(
                  icon: const Icon(Icons.arrow_drop_down),
                  onPressed: () => _showPicker(context, isWeight: true),
                ),
              ),
            ),
            const SizedBox(height: 20),
            TextField(
              controller: heightController,
              keyboardType: TextInputType.number,
              decoration: InputDecoration(
                labelText: "Height (cm)",
                border: const OutlineInputBorder(),
                suffixIcon: IconButton(
                  icon: const Icon(Icons.arrow_drop_down),
                  onPressed: () => _showPicker(context, isWeight: false),
                ),
              ),
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: _calculateBMI,
              style: ElevatedButton.styleFrom(
                backgroundColor: const Color(0xFFFFDADC),
                padding: const EdgeInsets.symmetric(vertical: 12),
              ),
              child: const Text(
                "Calculate BMI",
                style: TextStyle(color: Colors.red, fontSize: 20),
              ),
            ),
            const SizedBox(height: 20),
            if (_bmi != null) ...[
              Text(
                "Your BMI: ${_bmi!.toStringAsFixed(2)}",
                style: const TextStyle(
                  fontSize: 20,
                  fontWeight: FontWeight.bold,
                ),
              ),
              Text(
                _bmiCategory(_bmi!),
                style: const TextStyle(color: Colors.black54),
              ),
              const SizedBox(height: 30),
              const Text(
                "BMI Meter",
                style: TextStyle(fontSize: 18, fontWeight: FontWeight.w600),
              ),
              const SizedBox(height: 20),
              SizedBox(height: 250, child: _buildBMIGauge(_bmi!)),
            ],
          ],
        ),
      ),
    );
  }
}
