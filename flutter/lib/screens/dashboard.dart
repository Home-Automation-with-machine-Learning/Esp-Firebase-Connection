import 'package:flutter/material.dart';
import 'package:untitled/includes/drawer.dart';


class Dashboard extends StatefulWidget {
  const Dashboard({Key? key}) : super(key: key);

  @override
  _DashboardState createState() => _DashboardState();
}

class _DashboardState extends State<Dashboard> {
  int _value = 6;
  bool _lights = false;
  @override
  Widget build(BuildContext context) {


    return Scaffold(
      backgroundColor: Colors.white,
      drawer: NavDrawer(),
      appBar: AppBar(
        title: Text("Home Automation"),

      ),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: Text('Adjust Temperatures in your room', style: TextStyle(fontSize: 20),),
          ),
          Card(
            child: Slider(
                value: _value.toDouble(),
                min: 1.0,
                max: 10.0,
                divisions: 50,
                activeColor: Colors.red,
                inactiveColor: Colors.black,
                label: 'Adjust Temperatures',
                onChanged: (double newValue) {
                  setState(() {
                    _value = newValue.round();
                  });

                },

                semanticFormatterCallback: (double newValue) {

                  return '${newValue.round()} dollars';

                }

            ),
            elevation: 8,
            shadowColor: Colors.green,
            margin: EdgeInsets.all(20),
            shape:  OutlineInputBorder(
                borderRadius: BorderRadius.circular(10),
                borderSide: BorderSide(color: Colors.green, width: 1)
            ),
          ),
          Padding(
            padding: const EdgeInsets.all(8.0),
            child: Text('Turn bedroom lights on or off', style: TextStyle(fontSize: 20),),
          ),
          Card(
            child: SwitchListTile(
              title: const Text('Bedroom lights', style: TextStyle(fontSize:20),),
              subtitle: Text("Change the status of your room lighting"),
              value: _lights,
              onChanged: (bool value) {
                setState(() {
                  _lights = value;
                });
              },
              secondary: const Icon(Icons.lightbulb_outline),
            ),
            elevation: 8,
            shadowColor: Colors.green,
            margin: EdgeInsets.all(20),
            shape:  OutlineInputBorder(
                borderRadius: BorderRadius.circular(10),
                borderSide: BorderSide(color: Colors.black, width: 1)
            ),
          )
        ],
      ),

    );


  }
}
