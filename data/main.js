var scene, camera, renderer, axesHelper;

var WIDTH = window.innerWidth;
var HEIGHT = window.innerHeight;

var SPEED = 0.01;
var coord = [1, 2, 3];

function init() {
    scene = new THREE.Scene();
    scene.background = new THREE.Color(0x0000ff);
    axesHelper = new THREE.AxisHelper(100);
    scene.add(axesHelper);
    var plane = new THREE.GridHelper(150, 15);
    // plane.add(axesHelper);
    scene.add(plane);

    initCube();
    initCube2();
    initSphere();
    initCamera();
    initRenderer();
    assembleShapes();

    document.body.appendChild(renderer.domElement);
}

function initCamera() {
    camera = new THREE.PerspectiveCamera(45, WIDTH / HEIGHT, 1, 1000);
    camera.position.set(0, 200, 400);
    camera.lookAt(scene.position);
}

function initRenderer() {
    renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
    renderer.setSize(WIDTH, HEIGHT);
    renderer.setClearColor(0xff);
}

function initCube() {
    cube = new THREE.Mesh(new THREE.CubeGeometry(25, 25, 25), new THREE.MeshNormalMaterial());
    // cube.position.x = 100;
    // cube.position.y = 100;
    // cube.position.z = -3;
    cube.add(axesHelper);
}

function initCube2() {
    cube2 = new THREE.Mesh(new THREE.CubeGeometry(15, 50, 15), new THREE.MeshNormalMaterial());
}

function initSphere() {
    sphere = new THREE.Mesh(new THREE.SphereGeometry(12, 32, 32), new THREE.MeshNormalMaterial());
}

function assembleShapes() {
    cube.add(sphere);
    sphere.add(cube2);
    scene.add(cube);
}

function rotateCube() {
    // cube.rotation.x -= SPEED * 2;
    // cube.rotation.y -= SPEED;
    // cube.rotation.z -= SPEED * 3;
    cube.rotation.x -= coord(0);
    cube.rotation.y -= coord(1);
    cube.rotation.z -= coord(2);
}

function rotateCube2() {
    cube2.rotation.x = cube.rotation.x;
    cube2.rotation.y = cube.rotation.y;
    cube2.rotation.z = cube.rotation.z;
}

function rotateSphere() {
    sphere.rotation.x = cube.rotation.x;
    sphere.rotation.y = cube.rotation.y;
    sphere.rotation.z = cube.rotation.z;
}

setInterval(function ( ) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "/data", true);
    xhttp.send();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        //var x = (new Date()).getTime(),
          //  y = parseFloat(this.responseText);
        coord = Array.from(this.responseText.split(", "), Number);
          //chartP.series[0].addPoint([x, y], true, false, true);
      }
    };
  }, 800 ) ;

function offsetCube2() {
    var r = 50 / 2 + 12;
    // cube2.position.x = r * Math.cos(cube.rotation.x) * Math.sin(cube.rotation.y);
    // cube2.position.y = r * Math.sin(cube.rotation.x) * Math.sin(cube.rotation.y);
    // cube2.position.z = r * Math.cos(cube.rotation.y);
    cube2.position.y = r;
}

function offsetSphere() {
    var r = 12.5 + 12;
    sphere.position.y = r;
}

function render() {
    requestAnimationFrame(render);
    rotateCube();
    //rotateCube2();
    rotateSphere();
    offsetCube2();
    offsetSphere();
    renderer.render(scene, camera);
}

init();
render();
