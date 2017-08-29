const gulp = require("gulp");
const exec = require('child_process').exec;

var buildCommand = "spcomp chillyroll.sp -i include -o dist/chillyroll";

gulp.task('build', () =>{
    exec(buildCommand, (error, stdout, stderr) => {
        console.log(stdout);
        console.log(stderr);
    });
});

gulp.task('watch', () => {
    gulp.watch('chillyroll.sp', ['build']);
    gulp.watch('include/**/*', ['build']);
});